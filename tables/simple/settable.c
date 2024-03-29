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
    table => {
      "cat" => "Meow"
      "dog" => "Bow"
    }
  */
  lua_setglobal(L, "table");

  luaL_dostring(L, "print(table.cat)");
  luaL_dostring(L, "print(table.dog)");
  return 0;
}

  
