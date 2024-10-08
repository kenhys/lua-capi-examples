#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int main(void)
{
  lua_State *L = luaL_newstate();
  int status = 0;

  luaL_openlibs(L);

  lua_newtable(L);

  /*
    {
      "cat" => "Meow",
    }
  */
  lua_pushstring(L, "Meow");
  lua_setfield(L, -2, "cat");

  /*
    {
      "cat" => "Meow",
      "dog" => "Bow",
    }
  */
  lua_pushstring(L, "Bow");
  lua_setfield(L, -2, "dog");

  /*
    table => {
      "cat" => "Meow",
      "dog" => "Bow",
    }
  */
#if 1
  lua_setglobal(L, "animal");
#else
  lua_pushstring(L, "animal");
  lua_insert(L, -2); // align "table", {} order
  lua_settable(L, LUA_GLOBALSINDEX);
#endif

  status = luaL_dostring(L, "print(animal.cat)");
  if (status) {
    printf("Error: %s", lua_tostring(L, -1));
  }

  status = luaL_dostring(L, "print(animal.dog)");
  if (status) {
    printf("Error: %s", lua_tostring(L, -1));
  }
  luaL_dostring(L, "local inspect=require('inspect');print(inspect(animal))");
  lua_close(L);
  return 0;
}

  
