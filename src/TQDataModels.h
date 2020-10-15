//
// Created by Marek Piotrowski-EXT on 2020-10-11.
//

#ifndef TECHQUEST_TQDATAMODELS_H
#define TECHQUEST_TQDATAMODELS_H

#include <string>
#include <stdexcept>
#include <regex>
#include <nlohmann/json.hpp>



namespace gameType {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    struct InfluenceElement {
        int64_t id;
        double value;
    };

    struct Action {
        int64_t id;
        std::string name;
        std::string description;
        std::string type;
        std::vector<InfluenceElement> influence;
    };

    struct GameDataAttribute {
        int64_t id;
        std::string name;
        int64_t min;
        int64_t max;
    };

    struct Command {
        int64_t id;
        std::string command;
        std::string description;
    };

    struct Item {
        int64_t id;
        std::string name;
        std::string description;
        std::string resources;
        int64_t type;
        std::vector<InfluenceElement> actions;
    };

    struct Place {
        int64_t id;
        std::string name;
        std::string description;
        std::vector<int64_t> items;
        std::vector<InfluenceElement> actions;
    };

    struct Player {
        std::string name;
        std::vector<InfluenceElement> attributes;
    };

    struct State {
        int64_t placeIndex;
    };

    struct GameData {
        State state;
        std::vector<Place> places;
        std::vector<Action> actions;
        std::vector<Item> items;
        std::vector<GameDataAttribute> attributes;
        Player player;
        std::vector<Command> commands;
    };
}

namespace nlohmann {
    void from_json(const json & j, gameType::InfluenceElement & x);
    void to_json(json & j, const gameType::InfluenceElement & x);

    void from_json(const json & j, gameType::Action & x);
    void to_json(json & j, const gameType::Action & x);

    void from_json(const json & j, gameType::GameDataAttribute & x);
    void to_json(json & j, const gameType::GameDataAttribute & x);

    void from_json(const json & j, gameType::Command & x);
    void to_json(json & j, const gameType::Command & x);

    void from_json(const json & j, gameType::Item & x);
    void to_json(json & j, const gameType::Item & x);

    void from_json(const json & j, gameType::Place & x);
    void to_json(json & j, const gameType::Place & x);

    void from_json(const json & j, gameType::Player & x);
    void to_json(json & j, const gameType::Player & x);

    void from_json(const json & j, gameType::State & x);
    void to_json(json & j, const gameType::State & x);

    void from_json(const json & j, gameType::GameData & x);
    void to_json(json & j, const gameType::GameData & x);

    inline void from_json(const json & j, gameType::InfluenceElement& x) {
        x.id = j.at("id").get<int64_t>();
        x.value = j.at("value").get<double>();
    }

    inline void to_json(json & j, const gameType::InfluenceElement & x) {
        j = json::object();
        j["id"] = x.id;
        j["value"] = x.value;
    }

    inline void from_json(const json & j, gameType::Action& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.description = j.at("description").get<std::string>();
        x.type = j.at("type").get<std::string>();
        x.influence = j.at("influence").get<std::vector<gameType::InfluenceElement>>();
    }

    inline void to_json(json & j, const gameType::Action & x) {
        j = json::object();
        j["id"] = x.id;
        j["name"] = x.name;
        j["description"] = x.description;
        j["type"] = x.type;
        j["influence"] = x.influence;
    }

    inline void from_json(const json & j, gameType::GameDataAttribute& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.min = j.at("min").get<int64_t>();
        x.max = j.at("max").get<int64_t>();
    }

    inline void to_json(json & j, const gameType::GameDataAttribute & x) {
        j = json::object();
        j["id"] = x.id;
        j["name"] = x.name;
        j["min"] = x.min;
        j["max"] = x.max;
    }

    inline void from_json(const json & j, gameType::Command& x) {
        x.id = j.at("id").get<int64_t>();
        x.command = j.at("command").get<std::string>();
        x.description = j.at("description").get<std::string>();
    }

    inline void to_json(json & j, const gameType::Command & x) {
        j = json::object();
        j["id"] = x.id;
        j["command"] = x.command;
        j["description"] = x.description;
    }

    inline void from_json(const json & j, gameType::Item& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.description = j.at("description").get<std::string>();
        x.resources = j.at("resources").get<std::string>();
        x.type = j.at("type").get<int64_t>();
        x.actions = j.at("actions").get<std::vector<gameType::InfluenceElement>>();
    }

    inline void to_json(json & j, const gameType::Item & x) {
        j = json::object();
        j["id"] = x.id;
        j["name"] = x.name;
        j["description"] = x.description;
        j["resources"] = x.resources;
        j["type"] = x.type;
        j["actions"] = x.actions;
    }

    inline void from_json(const json & j, gameType::Place& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.description = j.at("description").get<std::string>();
        x.items = j.at("items").get<std::vector<int64_t>>();
        x.actions = j.at("actions").get<std::vector<gameType::InfluenceElement>>();
    }

    inline void to_json(json & j, const gameType::Place & x) {
        j = json::object();
        j["id"] = x.id;
        j["name"] = x.name;
        j["description"] = x.description;
        j["items"] = x.items;
        j["actions"] = x.actions;
    }

    inline void from_json(const json & j, gameType::Player& x) {
        x.name = j.at("name").get<std::string>();
        x.attributes = j.at("attributes").get<std::vector<gameType::InfluenceElement>>();
    }

    inline void to_json(json & j, const gameType::Player & x) {
        j = json::object();
        j["name"] = x.name;
        j["attributes"] = x.attributes;
    }

    inline void from_json(const json & j, gameType::State& x) {
        x.placeIndex = j.at("placeIndex").get<int64_t>();
    }

    inline void to_json(json & j, const gameType::State & x) {
        j = json::object();
        j["placeIndex"] = x.placeIndex;
    }

    inline void from_json(const json & j, gameType::GameData& x) {
        x.state = j.at("state").get<gameType::State>();
        x.places = j.at("places").get<std::vector<gameType::Place>>();
        x.actions = j.at("actions").get<std::vector<gameType::Action>>();
        x.items = j.at("items").get<std::vector<gameType::Item>>();
        x.attributes = j.at("attributes").get<std::vector<gameType::GameDataAttribute>>();
        x.player = j.at("player").get<gameType::Player>();
        x.commands = j.at("commands").get<std::vector<gameType::Command>>();
    }

    inline void to_json(json & j, const gameType::GameData & x) {
        j = json::object();
        j["state"] = x.state;
        j["places"] = x.places;
        j["actions"] = x.actions;
        j["items"] = x.items;
        j["attributes"] = x.attributes;
        j["player"] = x.player;
        j["commands"] = x.commands;
    }
}


#endif //TECHQUEST_TQDATAMODELS_H
