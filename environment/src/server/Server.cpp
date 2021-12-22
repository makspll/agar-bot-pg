#include "core/Player.h"
#include <iostream>
#include "math/math.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core/macros.h"
#include "server/Server.h"
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include "core/Packets.h"
#include "core/Logging.h"
#include "core/Timer.h"

Server::Server(int port){

  if((socket_in_fd = socket(AF_INET,SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP)) == -1){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port = htons(port);
  local_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(socket_in_fd, (sockaddr*)&local_address,sizeof(local_address)) == -1){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // initialize structures
  for(int i = 0; i < MAX_PLAYERS;i++){
    players[i] = Player();
  }

  
  // spin
  Spin();
  
}

void Server::Spin(){


  char buffer[BUFLEN];
  struct sockaddr_in from;
  int from_size = sizeof(from);
  
  // main loop
  Timer tick_loop;
  Timer sync_loop;

  tick_loop.start();
  sync_loop.start();


  while ( true ){
    
    auto elapsed_tick_ns = tick_loop.read_ns();
    auto elapsed_sync_ns = sync_loop.read_ns();

    // TICK
    if(elapsed_tick_ns >= TICK_NS){
      Logging::LOG(LOG_LEVEL::DEBUG, "Ticking simulation.");

      while( true ){
        int bytes_received = recvfrom(socket_in_fd,buffer,BUFLEN,0,(sockaddr*)&from,(socklen_t*)&from_size); 
        if (bytes_received == -1){
            if (errno == EAGAIN || errno == EWOULDBLOCK){
              break; // no more packets
            }
        } else {
          // read current packet 
          if (bytes_received < 1){
            continue; // ignore bad packets
          }  

          char pid = buffer[1];

          switch (buffer[0]){
            case (char)ClientPacket::JOIN:
            { 
              char result = (char)ServerPacket::RESULT_FAIL;
              char slot = -1;
              // find empty slot
              for(int i = ID::get_first_idx(EntityType::PLAYER_BLOB); i < MAX_PLAYERS + ID::get_first_idx(EntityType::PLAYER_BLOB); i++){
                  if(players[i].address == 0){
                    result = (char)ServerPacket::RESULT_SUCCESS;
                    slot = i;
                    break;
                }
              }
              buffer[0] = result;
              buffer[1] = slot;
              // send result, and track player
              if ((sendto(socket_in_fd,buffer,2,0, (sockaddr*)&from,from_size)) != -1){
                  if(slot != -1){
                    std::cout << "Player joined at slot: "<< (int)slot <<'\n';
                    players[slot].address = from.sin_addr.s_addr;
                    players[slot].port = from.sin_port;
                    players[slot].time_without_contact = -TICK_SECONDS;
                  } else {
                    std::cout << "Player attempted to join full server" <<'\n';
                  }
              }

              // register player to state
              state.add_player(slot);            
            }
            break;
            case (char)ClientPacket::LEAVE:
            {
              std::cout << "Player left from slot: " << (int)pid << '\n';
              players[pid].time_without_contact = -TICK_SECONDS;
              players[pid].address = 0;
              players[pid].port = 0;
              received_buffers[pid] = PacketBuffer<GameInput>(5);

              // unregister player from state
              state.remove_player(pid);            
            }
            break;
            case (char)ClientPacket::INPUT:
            {
              players[pid].time_without_contact = -TICK_SECONDS;
              unsigned int seq_number;
              memcpy(&seq_number,buffer + 2,sizeof(unsigned int));
              auto a = GameInput();
              a.deserialize(buffer + CLIENT_HEADER_LEN);
              // each client may send multiple inputs per tick, we want to only process the most recent one
              // client's are usually in the "future" since they sent the input, we dictate the "present"
              received_buffers[pid].enqueue(a,seq_number);
            }
            break;
          }
        }
      }

      /// update game state

      // first load all client's most recent inputs
      // then update them
      for(int i = 0; i < MAX_PLAYERS; i++){
        if(players[i].address != 0 && received_buffers[i].size() > 0) {
          players[i].last_processed_sequence_number = received_buffers[i].iterator()->seq_number;
          auto a = received_buffers[i].pop();
          state.set_input(i,a);
          state.update_player(TICK_SECONDS,i);
        }
      }

      // collision checks, food spawns etc
      state.update_world(TICK_SECONDS);

      tick_loop.start();
    }


    // SYNC

    if(elapsed_sync_ns >= STATE_SYNC_NS){
      Logging::LOG(LOG_LEVEL::DEBUG, "Synchronising clients.\n");
      
      struct sockaddr_in client_address;
      client_address.sin_family = AF_INET;
      int client_add_len = sizeof(client_address);

      for(int i = 0; i < MAX_PLAYERS; i++){
        if(players[i].address != 0){
          buffer[0] = (char)ServerPacket::GAME_STATE;
          memcpy(buffer+1,&players[i].last_processed_sequence_number,sizeof(unsigned int));
          int bytes_written = state.to(buffer+SERVER_HEADER_LEN ,i);

          client_address.sin_port = players[i].port;
          client_address.sin_addr.s_addr = players[i].address;
          if ((sendto(socket_in_fd,buffer,bytes_written + SERVER_HEADER_LEN ,0, (sockaddr*)&client_address,client_add_len)) == -1){
            // error
            perror("error sending state");
          }

          players[i].time_without_contact += TICK_SECONDS;
        }
      }

      sync_loop.start();
    }

    // wait for next coming tick
    long double diff_tick_s = TICK_SECONDS  - (elapsed_tick_ns / 1e9);
    long double diff_sync_s = STATE_SYNC_SECONDS - (elapsed_sync_ns / 1e9);
    long double wait_time_s = std::min(diff_sync_s,diff_tick_s);

    if (wait_time_s > 0){
      usleep((wait_time_s) * 1e6);
    }
  }
  
}

