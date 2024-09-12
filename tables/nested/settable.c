#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void dump(lua_State *L)
{
  int size = lua_gettop(L);
  printf("stack size: %d\n", size);
  for (int i = size; i >= 1; i--) {
    int type = lua_type(L, i);
    printf("%2d: (%6s) ", i - size - 1, lua_typename(L,type));
    switch ( type ) {
    case LUA_TNUMBER:
      printf("%d\n", lua_tonumber(L, i));
      break;
    case LUA_TSTRING:
      printf("%s\n", lua_tostring(L, i));
      break;
    case LUA_TTABLE:
      printf("{}\n");
      break;
    }
  }
}

int main(void)
{
  lua_State *L = luaL_newstate();
  int status = 0;

  luaL_openlibs(L);
  lua_newtable(L);

#if 1
  /* Originally problem: https://www.clear-code.com/blog/2020/11/11.html */
  lua_pushstring(L, "animal"); /* {}, animal */
  lua_newtable(L); /* {}, animial, {} */
  lua_pushstring(L, "cat"); /* {}, animal, {}, cat */
  lua_pushstring(L, "Meow"); /* {}, animal, {}, cat, Meow */
  dump(L);
  lua_settable(L, -3); /* {}, animal, { cat = Meow } */
  dump(L);
  lua_settable(L, -3); /* { animal => { cat => Meow }} */
  dump(L);
  lua_setglobal(L, "table");
  dump(L);
  luaL_dostring(L, "local inspect=require('inspect'); print(inspect(table))");
#else
  /* Use lua_insert */
  /*
    {
    "cat" => "Meow"
    }
  */
  lua_pushstring(L, "cat");
  lua_pushstring(L, "Meow");
  lua_settable(L, -3);

  /*
    animal => {
      "cat" => "Meow"
    }
  */
  lua_pushstring(L, "animal");
  lua_insert(L, -2); /* animal, {} */

  /*
    { animal => { "cat" => "Meow" } }
   */
  lua_newtable(L); /* animal, {}, {} */
  lua_insert(L, -3); /* {}, animal, {} */
  lua_settable(L, -3);

  lua_pushstring(L, "table"); /* {}, table */
  lua_insert(L, -2);
  lua_settable(L, LUA_GLOBALSINDEX);

  luaL_dostring(L, "local inspect=require('inspect'); print(inspect(table))");

#endif
  lua_close(L);
  return 0;
}
