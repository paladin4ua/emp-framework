//
// Created by paladin on 2/7/2017.
//

#ifndef EMP_FRAMEWORK_TEST_MODEL_H
#define EMP_FRAMEWORK_TEST_MODEL_H

#include <boost/optional.hpp>
#include <vector>

struct Planet_Resources {
    typedef double value_t;
    value_t iron;
    value_t polymer;
    value_t radium;
    template <class T>
    void bind(T & t) {
      t.bind(iron, "iron");
      t.bind(polymer, "polymer");
      t.bind(radium, "radium");
    }
};

struct Placeholder {
    int8_t building_id;
    int8_t building_level;
    int8_t status;
    int8_t type;
    template <class T>
    void bind(T & t) {
      t.bind(building_id, "building_id");
      t.bind(building_level, "building_level");
      t.bind(status, "status");
      t.bind(type, "type");
    }
};

struct Planet_Desc
  {
    boost::optional<int32_t> owner;
    int32_t star_system;
    int32_t orbit;
    int32_t prototype;
    int32_t engadged_workers;
    int32_t available_workers;
    int32_t hideout_size;
    std::string name;

    std::vector<Placeholder> placeholders;
    Planet_Resources available_resources;
    Planet_Resources orbital_resources;

    Planet_Resources::value_t space_junk_refinary_rate;

    boost::optional<int> building_in_process_placeholder_ind;
    boost::optional<int64_t > building_in_process_time_to_finish;
    boost::optional<int32_t> building_in_process_item_id;

    Planet_Resources farmed_resources;
    Planet_Resources farmed_orbital_resources;


    double loyalty;
    int32_t last_invader_id;
    int32_t time_to_destroy_space_base;

    Planet_Resources free_resources;
    Planet_Resources last_planet_minings;


    template <class T>
    void bind(T & t) {
      t.bind(owner , "owner");
      t.bind(star_system , "star_system");
      t.bind(orbit , "orbit");
      t.bind(prototype , "prototype");
      t.bind(engadged_workers , "engadged_workers");
      t.bind(available_workers , "available_workers");
      t.bind(hideout_size , "hideout_size");
      t.bind(name , "name");
      t.bind(placeholders , "placeholders");
      t.bind(available_resources , "available_resources");
      t.bind(orbital_resources , "orbital_resources");
      t.bind(space_junk_refinary_rate , "space_junk_refinary_rate");
      t.bind(building_in_process_placeholder_ind , "building_in_process_placeholder_ind");
      t.bind(building_in_process_time_to_finish , "building_in_process_time_to_finish");
      t.bind(building_in_process_item_id , "building_in_process_item_id");
      t.bind(farmed_resources , "farmed_resources");
      t.bind(farmed_orbital_resources , "farmed_orbital_resources");
      t.bind(loyalty , "loyalty");
      t.bind(last_invader_id , "last_invader_id");
      t.bind(time_to_destroy_space_base , "time_to_destroy_space_base");
      t.bind(free_resources , "free_resources");
      t.bind(last_planet_minings, "last_planet_minings");

    }
  };

int32_t random(int max) {
  return rand() % max;
}

Planet_Resources random_res(int max) {
  Planet_Resources r;
  r.iron = random(max);
  r.radium = random(max);
  r.polymer = random(max);
  return r;
}

struct large_model {
    std::string name;
    std::vector<Planet_Desc> planets;
    template <class T>
    void bind(T & t) {
        t.bind(name, "name");
        t.bind(planets, "planets");
    }
};

large_model build_test_model(int planets_count) {

    large_model lm;

    lm.name = "large_model";
    lm.planets.resize(planets_count);
  for (auto & planet : lm.planets) {


    planet.owner = random(1000000);
    planet.star_system = random(1000000);
    planet.orbit = random(5);
    planet.prototype = random(100);
    planet.engadged_workers = random(10000);
    planet.available_workers = random(10000);
    planet.hideout_size = random(100);
    planet.name = "name";
    planet.placeholders.resize(random(50));
    for (auto & p: planet.placeholders) {
      p.building_id = random(100);
      p.building_level = random(20);
      p.type = random(10);
      p.status = random(10);
    }

    planet.available_resources = random_res(100000);
    planet.orbital_resources = random_res(100000);
    planet.space_junk_refinary_rate = random(10000);
    planet.building_in_process_placeholder_ind = random(10);
    planet.building_in_process_time_to_finish = random(10000);
    planet.building_in_process_item_id = random(100);
    planet.farmed_resources = random_res(100000);
    planet.farmed_orbital_resources = random_res(1000000);
    planet.loyalty = random(10000);
    planet.last_invader_id = random(10000);
    planet.time_to_destroy_space_base = random(10000);
    planet.free_resources = random_res(1000000);
    planet.last_planet_minings = random_res(1000000);
  }
    return lm;
}


#endif //EMP_FRAMEWORK_TEST_MODEL_H

