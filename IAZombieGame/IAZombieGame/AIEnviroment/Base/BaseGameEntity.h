#pragma once
#include <glm/ext/vector_float3.hpp>
#pragma once  
#include <glm/ext/vector_float3.hpp>  
#include <string>  

class BaseGameEntity  
{  
private:  
   int m_entityID;  
   static int m_nextID;  

public:  
   BaseGameEntity() : m_entityID(-1) {}  
   virtual ~BaseGameEntity() = default;  

   virtual std::string GetType() const = 0;  
   virtual void Update() = 0;  

   int GetID() const { return m_entityID; }  

   void SetID(int id) {  
       m_entityID = id;  
   }  
};
/*
*Entities AI have a unique ID because all entity
*is different, since they are all different and need 
*a unique ID to move and have diffents ways to think
*/