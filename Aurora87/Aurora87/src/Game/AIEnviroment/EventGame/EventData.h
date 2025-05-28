#pragma once
#include<string>
#include<map>
#include<variant>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"  
#include "glm/gtc/type_ptr.hpp"  
namespace AIEnviroment
{
	//Data Types the event system will use and send.
	using EventParameter = std::variant<int, float, bool, std::string>;
	//EventParameter is a variant type that can hold different types of data.
	using ParameterMap = std::map<std::string, EventParameter>;

	//EventType is an enum class that defines the different types of events that can occur in the game.
	enum class EventType {
		NONE,
		PLAYER_CHANGE_GUN,
		AK47_SHOOT,
		PLAYER_REGISTER,
		PLAYER_SPEED,
		ZOMBIE_CHARGE_ATTACK,
		CUBE_BRICK,
		CUBE_TEXTURE,
		PLAYER_ROTATE,
		RESIZE_CAMERA,
		ZOMBIE_SPAWN,
		PLAYER_PUNCH,
		RANDOM_SPAWN,
		PLAYER_DOWN,
		PLAYER_MOVE,
		PLAYER_RUN,
		PLAYER_ATTACK,
		PLAYER_JUMP,
		ZOMBIE_MOVE,
		ZOMBIE_ATTACK,
		ZOMBIE_DEATH,
		PLAYER_DEATH,
		PLAYER_RESPAWN,
		PLAYER_PICKUP_ITEM,
		ZOMBIE_PICKUP_ITEM,
		PLAYER_USE_ITEM,
		ZOMBIE_USE_ITEM,
		PLAYER_INTERACT,
		ZOMBIE_INTERACT,
		PLAYER_HEALTH,
		GAME_START,
		GAME_PAUSE,
		GAME_OVER,
		GAME_WIN,
		ZOMBIE_LEVEL_UP,
		ZOMBIE_PERSUIT,
		ZOMBIE_SEEK,
		GAME_SOUND_LOW,
		GAME_SOUND_HIGH,
	};

	//EventData is a class that holds the data for an event. It contains the event type and a map of parameters.
	struct EventData {
		EventType type;
		ParameterMap parameters;
		EventData(EventType eventType) : type(eventType) {}

		template<typename T>
		T Get(const std::string& key) const {
			if (parameters.count(key)) {
				if (std::holds_alternative<T>(parameters.at(key))) {
					return std::get<T>(parameters.at(key));
				}
				else {
					throw std::bad_variant_access();
				}
			}
			else {
				return T(); // Return default value if key not found
			}
		}
		//Function to check if the key exists in the parameters map.
		bool HasKey(const std::string& key) const {
			return parameters.find(key) != parameters.end();
		}
		//Function to check if the key exists and is of the correct type.
		template<typename T>
		bool HasKeyOfType(const std::string& key) const {
			return HasKey(key) && std::holds_alternative<T>(parameters.at(key));
		}
		//Functions to get the parameters of different types.Can be modified to add more types in the future.
		int GetInt(const std::string& key) const {
			return Get<int>(key);
		}
		float GetFloat(const std::string& key) const {
			return Get<float>(key);
		}
		bool GetBool(const std::string& key) const {
			return Get<bool>(key);
		}
		std::string GetString(const std::string& key) const {
			return Get<std::string>(key);
		}

		//Function to set the parameters of different types.
		void Set(const std::string& key, const EventParameter& value) {
			parameters[key] = value;
		}
	};

	//
}