#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int main(void)
{
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  lua_newtable(L);

  /*
    {
    "cat" => "Meow"
    }
  */
  lua_pushstring(L, "cat");
  lua_pushstring(L, "Meow");
  lua_settable(L, -3);

  /*
    {
      "cat" => "Meow"
      "dog" => "Bow"
    }
  */
  lua_pushstring(L, "dog");
  lua_pushstring(L, "Bow");
  lua_settable(L, -3);

  /*
    animal => {
      "cat" => "Meow"
      "dog" => "Bow"
    }
  */
  lua_pushstring(L, "animal");
  lua_insert(L, -2);
  lua_settable(L, LUA_GLOBALSINDEX);

  luaL_dostring(L, "print(animal.cat)");
  luaL_dostring(L, "print(animal.dog)");
  lua_close(L);
  return 0;
}

  
