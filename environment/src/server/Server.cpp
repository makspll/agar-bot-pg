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
  int flags = 0;
  struct sockaddr_in from;
  int from_size = sizeof(from);
  
  // main loop
  while ( true ){
    struct timespec tick_start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick_start_time);

    // packet reading
    while( true ){

      int bytes_received = recvfrom(socket_in_fd,buffer,BUFLEN,flags,(sockaddr*)&from,(socklen_t*)&from_size); 

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
            for(int i = 0; i < MAX_PLAYERS; i++){
                if(players[i].address == 0){
                  result = (char)ServerPacket::RESULT_SUCCESS;
                  slot = i;
                  break;
              }
            }
            buffer[0] = result;
            buffer[1] = slot;
            // send result, and track player
            if ((sendto(socket_in_fd,buffer,2,flags, (sockaddr*)&from,from_size)) != -1){
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

            // unregister player from state
            state.remove_player(pid);            
          }
          break;
          case (char)ClientPacket::INPUT:
          {
            players[pid].time_without_contact = -TICK_SECONDS;
            auto a = GameInput();
            a.deserialize(buffer + CLIENT_HEADER_LEN);
            state.set_input(pid,a);
          }
          break;
          case (char)ClientPacket::GAME_STATE:
          {
            players[pid].time_without_contact = -TICK_SECONDS;
            PlayerState* pgs = state.get_player_state(pid);
            pgs->deserialize(buffer + CLIENT_HEADER_LEN);

          }
          break;
        }
      }
    }

    // update game state
    simulator.update(state, TICK_SECONDS);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    int client_add_len = sizeof(client_address);

    // send game state to clients
    Logging::LOG(LOG_LEVEL::DEBUG,"Server State: ");
    Logging::LOG(LOG_LEVEL::DEBUG,state);

    for(int i = 0; i < MAX_PLAYERS; i++){
      if(players[i].address != 0){
        buffer[0] = (char)ServerPacket::GAME_STATE;
        int bytes_written = state.to(buffer+SERVER_HEADER_LEN ,i);
        client_address.sin_port = players[i].port;
        client_address.sin_addr.s_addr = players[i].address;
        if ((sendto(socket_in_fd,buffer,bytes_written + SERVER_HEADER_LEN ,flags, (sockaddr*)&client_address,client_add_len)) == -1){
          // error
          perror("error sending state");
        }

        players[i].time_without_contact += TICK_SECONDS;
      }
    }

    // wait for next tick
    struct timespec tick_end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick_end_time);
    long diff_s = (tick_end_time.tv_sec - tick_start_time.tv_sec) 
      + (tick_end_time.tv_nsec - tick_start_time.tv_nsec) / 1e9;

    if (diff_s < TICK_SECONDS){
      usleep((TICK_SECONDS - diff_s) / 1e-6);
    }
  }
  
}

