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

  while (window->isOpen())
  { // main loop

    // check available state packets
    do{
      int bytes_received = recvfrom(socket_in_fd,buffer,BUFLEN,MSG_DONTWAIT,(sockaddr*)&server_address,&server_add_len); 
      if (bytes_received == -1){
          if (errno != EAGAIN && errno != EWOULDBLOCK){
            // error!
            perror("Error in retrieving state");
          }
      } else {
        // set state to most recent
        state.from(buffer + SERVER_HEADER_LEN, own_pid);
        received_first_state = true;

        Logging::LOG(LOG_LEVEL::DEBUG,"Received first state:");
        Logging::LOG(LOG_LEVEL::DEBUG,state);
      }

    } while (!received_first_state);


    // draw state
    struct timespec tick_start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick_start_time);

    // update our input
    sf::Event event;
    GameInput input;

    auto mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    // std::cout << mouse_pos.x <<','<< mouse_pos.y << '(' << mouse_pos.x / CLIENT_UNIT_LENGTH << ',' << mouse_pos.y / CLIENT_UNIT_LENGTH <<  '\n';

    input.mouse_pos[0] = mouse_pos.x;
    input.mouse_pos[1] = mouse_pos.y;

    while (window->pollEvent(event))
    { 
      if (event.type == sf::Event::Closed)
          window->close();

    }
    
    state.set_input(own_pid,input);
    
    // send own state to server
    buffer[0] = (char)ClientPacket::GAME_STATE;
    buffer[1] = (char)own_pid;
    int bytes_written = state.get_player_state(own_pid)->serialize(buffer + CLIENT_HEADER_LEN);
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
    std::cout << nearest_unit_tl << nearest_unit_br << '\n';

    for(int y = nearest_unit_tl[1]; y <= nearest_unit_br[1];y += CLIENT_UNIT_LENGTH){
      // draw line down from current x
      std::cout << y << ',';
      sf::RectangleShape line_down(sf::Vector2f((nearest_unit_br[0] - nearest_unit_tl[0]),1));
      line_down.setPosition(nearest_unit_tl[0],y);
      window->draw(line_down);
    }
      std::cout << '\n';

    for(int x = nearest_unit_tl[0]; x <= nearest_unit_br[0];x = x + CLIENT_UNIT_LENGTH){
      // draw line down from current x
      std::cout << x << ',';
      sf::RectangleShape line_down(sf::Vector2f(1,(nearest_unit_br[1] - nearest_unit_tl[1])));
      line_down.setPosition(x,nearest_unit_tl[1]);
      window->draw(line_down);
    }
      std::cout << '\n';


    // for(Vec2Int i = nearest_unit_tl; (i[0] <= nearest_unit_br[0]) && (i[1] <= nearest_unit_br[1]);i = i + CLIENT_UNIT_LENGTH){
    //   // draw line down from current x
    //   std::cout << i << '\n';
    //   sf::RectangleShape line_down(sf::Vector2f(1,(nearest_unit_br[1] - nearest_unit_tl[1])));
    //   line_down.setPosition(i[0],nearest_unit_tl[1]);
    //   window->draw(line_down);

    //   // draw line right from current y
    //   sf::RectangleShape line_right(sf::Vector2f((nearest_unit_br[0] - nearest_unit_tl[0]),1));
    //   line_right.setPosition(nearest_unit_tl[0],i[1]);
    //   window->draw(line_right);
    // }

    state.draw(window);

    window->display();
    
    // update state
    simulator.update(state,TICK_CLIENT_SECONDS);

    our_state = state.get_player_state(own_pid);

    // adjust window
    view.setCenter(our_state->pos[0],our_state->pos[1]);
    view.setSize(our_state->view_scale * CLIENT_INIT_VISIBLE_UNITS * CLIENT_UNIT_LENGTH,
      our_state->view_scale * CLIENT_INIT_VISIBLE_UNITS * CLIENT_UNIT_LENGTH);
    window->setView(view);

    
    Logging::LOG(LOG_LEVEL::DEBUG,"Updated state:");
    Logging::LOG(LOG_LEVEL::DEBUG,state);

    // wait for next tick/frame

    struct timespec tick_end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick_end_time);

    long diff_s = (tick_end_time.tv_sec - tick_start_time.tv_sec) 
      + (tick_end_time.tv_nsec - tick_start_time.tv_nsec) / 1e9;

    if (diff_s < TICK_CLIENT_SECONDS){
      usleep((TICK_CLIENT_SECONDS - diff_s) / 1e-6);
    }


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
