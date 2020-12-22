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

  lua_pushstring(L, "cat");

  lua_newtable(L);
  lua_pushstring(L, "Meow");

  lua_pushstring(L, "Meoow!");

  lua_settable(L, -3);

  lua_settable(L, -3);

  lua_setglobal(L, "table");

  luaL_dostring(L, "local inspect = require('inspect');print(inspect(table))");
  luaL_dostring(L, "print(table.cat.Meow)");
  return 0;
}

  
