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
#include <arpa/inet.h>
#include "client/Client.h"
#include "core/Packets.h"
#include <time.h>
#include <unistd.h>
#include <fcntl.h> 
#include "core/Logging.h"
#include "core/Timer.h"

Client::Client(int port, std::string ip){

  if((socket_in_fd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP)) == -1){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_add_len = sizeof(server_address);
  inet_pton(AF_INET,ip.c_str(),&server_address.sin_addr);

  // make join request
  char buffer[BUFLEN];
  buffer[0] = (char)ClientPacket::JOIN;
  int flags = 0;
  if(sendto(socket_in_fd, buffer, 1,flags,(sockaddr*)&server_address,server_add_len) == -1){
    perror("send failed");
    exit(EXIT_FAILURE);
  }



  // await response
  int attempts = 3;
  while(attempts > 0){
    int bytes_received = recvfrom(socket_in_fd,buffer,BUFLEN,0,(sockaddr*)&server_address,&server_add_len); 
    if(bytes_received == -1){
      perror("attempt failed.");
    } else {
      break;
    }
    attempts -=1 ;
    std::cout << "another attempt to connect..." << '\n';
    usleep((CLIENT_CONNECTION_RETRY_PERIOD) / 1e-6);
  }

  // set non-blocking mode
  flags = fcntl(socket_in_fd, F_GETFD);
  flags |= O_NONBLOCK;
  fcntl(socket_in_fd,F_SETFL,flags);


  if (attempts <= 0){
    std::cout << "could not connect to server." << '\n';
    return;
  }

  if (buffer[0] == (char)ServerPacket::RESULT_FAIL){
    std::cout << "server full." << '\n';
    return;
  } else {
    own_pid = buffer[1];
  }
  // spin

  spin();

  delete window;

}

void Client::spin(){
  // launch window

  window = new sf::RenderWindow(sf::VideoMode(1500, 1500), "Agario clone" + std::to_string(own_pid));
  PlayerState * our_state;
  sf::View view(sf::Vector2f(0.0f,0.0f),
    sf::Vector2f(CLIENT_INIT_VISIBLE_UNITS*CLIENT_UNIT_LENGTH,
      CLIENT_INIT_VISIBLE_UNITS*CLIENT_UNIT_LENGTH));

  char buffer[BUFLEN];

  bool received_first_state = false;

  Timer render_loop;
  Timer lag_timer;
  unsigned int curr_sequence_num = next_sequence_number++;

  while (window->isOpen())
  { // main loop

    render_loop.start();

    // check available state packets
    do{
      lag_timer.start();
      int bytes_received = recvfrom(socket_in_fd,buffer,BUFLEN,MSG_DONTWAIT,(sockaddr*)&server_address,&server_add_len); 
      if (bytes_received == -1){
          if (errno != EAGAIN && errno != EWOULDBLOCK){
            // error!
            perror("Error in retrieving state");
          }
      } else {
        lag_ms = lag_timer.read_ns() / 1e6;
        memcpy(&last_received_sequence_number,buffer + 1,sizeof(unsigned int));
        state.from(buffer + SERVER_HEADER_LEN);

        received_buffer.enqueue(state,last_received_sequence_number);
        // reconcille received state
        // replay inputs from last received sequence number
        if(sent_buffer.size() > 0){
          std::cout << "LAST: " << last_received_sequence_number << '\n';
          sent_buffer.erase_before(last_received_sequence_number + 1);
          for(auto it = sent_buffer.iterator(); it != sent_buffer.end();it++){
            std::cout << "APPLYING: " << it->seq_number << '\n';
            state.set_input(own_pid,it->msg);
            state.update_player(TICK_SECONDS,own_pid);
          }
        }

        received_first_state = true;
        render_loop.start(); // restart render loop, i.e. sync time with  server


        // Logging::LOG(LOG_LEVEL::DEBUG,"Received Game Input:");
        // Logging::LOG(LOG_LEVEL::DEBUG,state);
      

      }

    } while (!received_first_state);


    // update our input
    sf::Event event;
    GameInput input;

    auto mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    input.mouse_pos[0] = mouse_pos.x;
    input.mouse_pos[1] = mouse_pos.y;

    while (window->pollEvent(event))
    { 
      if (event.type == sf::Event::Closed)
          window->close();
      if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Escape){
          window->close();
        }
      }
    }
    state.set_input(own_pid,input);
    
    // send input to server
    // keep track of history for reconciliation with received input
    sent_buffer.enqueue(input,curr_sequence_num);
    buffer[0] = (char)ClientPacket::INPUT;
    buffer[1] = (char)own_pid;

    memcpy(buffer + 2, &curr_sequence_num,sizeof(unsigned int));
    int bytes_written = input.serialize(buffer + CLIENT_HEADER_LEN);
    if(sendto(socket_in_fd, buffer, bytes_written + CLIENT_HEADER_LEN,0,(sockaddr*)&server_address,server_add_len) == -1){
      // error
      perror("sending state to server failed.");
    };

    window->clear();
    
    // draw game
    
    // draw grid
    Vec2 top_left = Vec2(view.getCenter().x - view.getSize().x/2,view.getCenter().y - view.getSize().y/2) - (CLIENT_UNIT_LENGTH);
    Vec2 bot_right = Vec2(view.getCenter().x + view.getSize().x/2,view.getCenter().y + view.getSize().y/2) + (CLIENT_UNIT_LENGTH);
    Vec2Int nearest_unit_tl = ((Vec2Int)(top_left / CLIENT_UNIT_LENGTH)) * CLIENT_UNIT_LENGTH;
    Vec2Int nearest_unit_br = ((Vec2Int)(bot_right / CLIENT_UNIT_LENGTH)) * CLIENT_UNIT_LENGTH;

    nearest_unit_tl[0] = std::max(nearest_unit_tl[0],-(ARENA_HALF_WIDTH_UNITS) * (CLIENT_UNIT_LENGTH));
    nearest_unit_tl[1] = std::max(nearest_unit_tl[1],-(ARENA_HALF_WIDTH_UNITS) * (CLIENT_UNIT_LENGTH));
    nearest_unit_br[0] = std::min(nearest_unit_br[0],(ARENA_HALF_WIDTH_UNITS) * (CLIENT_UNIT_LENGTH));
    nearest_unit_br[1] = std::min(nearest_unit_br[1],(ARENA_HALF_WIDTH_UNITS) * (CLIENT_UNIT_LENGTH));

    for(int y = nearest_unit_tl[1]; y <= nearest_unit_br[1];y += CLIENT_UNIT_LENGTH){
      // draw line down from current x
      sf::RectangleShape line_down(sf::Vector2f((nearest_unit_br[0] - nearest_unit_tl[0]),1));
      line_down.setPosition(nearest_unit_tl[0],y);
      window->draw(line_down);
    }

    for(int x = nearest_unit_tl[0]; x <= nearest_unit_br[0];x = x + CLIENT_UNIT_LENGTH){
      // draw line down from current x
      sf::RectangleShape line_down(sf::Vector2f(1,(nearest_unit_br[1] - nearest_unit_tl[1])));
      line_down.setPosition(x,nearest_unit_tl[1]);
      window->draw(line_down);
    }



    state.draw(window);


    window->display();

    // update state, 
    state.update_player(TICK_SECONDS, own_pid);

    // interpolate using the buffer up to its capacity at most from the first stored snapshot
    unsigned int ticks_since_sync = curr_sequence_num - last_received_sequence_number - 1; // we need to be one ahead for interpolation
    if(received_buffer.size() >= 2 && ticks_since_sync > 0){
      float t = ticks_since_sync / INTERPOLATION_TICKS;
      t = std::min(t,1.0f);

      GameState prev_received_state = (++received_buffer.iterator())->msg;
      GameState prev_prev_received_state = (received_buffer.iterator())->msg;
      
      state.interpolate_all_but(own_pid,&prev_prev_received_state,&prev_received_state,t);
    }

    our_state = state.get_player_state(own_pid);

    // adjust window
    view.setCenter(our_state->pos[0],our_state->pos[1]);
    view.setSize(our_state->view_scale * CLIENT_INIT_VISIBLE_UNITS * CLIENT_UNIT_LENGTH,
      our_state->view_scale * CLIENT_INIT_VISIBLE_UNITS * CLIENT_UNIT_LENGTH);
    window->setView(view);


    // Logging::LOG(LOG_LEVEL::DEBUG,"Updated state:");
    // Logging::LOG(LOG_LEVEL::DEBUG,state);


    // wait for next tick/frame
    long double wait_time_s = TICK_SECONDS - (render_loop.read_ns() / 1e9);
    if (wait_time_s > 0){
      usleep(wait_time_s * 1e6);
    }

    curr_sequence_num++;
  }

  // window closed
  // exit server gracefully
  buffer[0] = (char)ClientPacket::LEAVE;
  buffer[1] = own_pid;
  int flags = 0;
  if(sendto(socket_in_fd, buffer, CLIENT_HEADER_LEN,flags,(sockaddr*)&server_address,server_add_len) == -1){
    perror("send failed");
    exit(EXIT_FAILURE);
  }
  std::cout << "disconnected from server." << '\n';

  
};
