#include "gtest/gtest.h"
#include "math/math.h"
#include "state/FoodPelletState.h"
#include "state/BlobState.h"
#include "state/PlayerState.h"
#include "core/GameState.h"

TEST(SerializationTests, Vector) {
  char buffer[20];
  Vec2Int a = Vec2Int(0,1);
  Vec2Int b;

  int bytes = a.serialize(buffer);
  int bytes2 = b.deserialize(buffer);

  ASSERT_EQ(bytes,bytes2);
  ASSERT_EQ(a[0],b[0]);
  ASSERT_EQ(a[1],b[1]);
}

TEST(SerializationTests, FoodPelletState) {
  char buffer[20];
  FoodPelletState a = FoodPelletState();
  a.id.entity = 5;
  a.pos = Vec2(5,5);
  FoodPelletState b;
  
  int bytes = a.serialize(buffer);
  int bytes2 = b.deserialize(buffer);

  ASSERT_EQ(bytes,bytes2);
  ASSERT_EQ(a.id.entity,b.id.entity);
  ASSERT_FLOAT_EQ(a.pos[0],b.pos[0]);
  ASSERT_FLOAT_EQ(a.pos[1],b.pos[1]);
}

TEST(SerializationTests, BlobState) {
  char buffer[20];
  BlobState a = BlobState();
  a.set_mass(5);
  a.pos = Vec2(5,5);
  BlobState b;
  
  int bytes = a.serialize(buffer);
  int bytes2 = b.deserialize(buffer);

  ASSERT_EQ(bytes,bytes2);
  ASSERT_EQ(a.mass,b.mass);
  ASSERT_EQ(a.radius,b.radius);
  ASSERT_FLOAT_EQ(a.pos[0],b.pos[0]);
  ASSERT_FLOAT_EQ(a.pos[1],b.pos[1]);
}

TEST(SerializationTests, PlayerState) {
  char buffer[20];
  PlayerState a = PlayerState(5);

  a.view_scale = 2;
  PlayerState b;
  
  int bytes = a.serialize(buffer);
  int bytes2 = b.deserialize(buffer);

  ASSERT_EQ(bytes,bytes2);
  ASSERT_EQ(a.view_scale,b.view_scale);
  ASSERT_EQ(a.parts.size(),b.parts.size());
}

TEST(SerializationTests, GameState) {
  char buffer[200];
  GameState a = GameState();
  a.add_player(0);
  a.add_food(0);
  a.add_food(1);
  a.add_food(2);
  a.add_food(3);
  a.add_food(4);
  a.add_food(5);
  a.add_food(6);
  a.add_food(7);
  a.add_food(8);
  a.add_food(9);
  GameState b;

  int bytes = a.to(buffer,0);
  int bytes2 = b.from(buffer);

  ASSERT_EQ(bytes,bytes2);
  for(auto const&x : a.players){
      ASSERT_EQ(a.players[x.first],b.players[x.first]);
    
  }
  for(auto const&x : a.food_pellets){
      ASSERT_EQ(a.food_pellets[x.first],b.food_pellets[x.first]);
  }
}
