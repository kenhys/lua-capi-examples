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

  lua_pushstring(L, "Meow");
  lua_setfield(L, -2, "cat");

  lua_pushstring(L, "Bow");
  lua_setfield(L, -2, "dog");

  lua_setglobal(L, "table");

  status = luaL_dostring(L, "print(table.cat)");
  if (status) {
    printf("Error: %s", lua_tostring(L, -1));
  }

  status = luaL_dostring(L, "print(table.dog)");
  if (status) {
    printf("Error: %s", lua_tostring(L, -1));
  }
  return 0;
}

  
