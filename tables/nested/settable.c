#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define BLOG20201111 0

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
      /* dump table */
      lua_pushnil(L);
      while (lua_next(L, i) != 0) {
        printf("(%s) %s - (%s) %s\n",
               lua_typename(L, lua_type(L, -2)), lua_tostring(L, -2), lua_typename(L, lua_type(L, -1)), lua_tostring(L, -1));
        // pop only value, reuse key
        lua_pop(L, 1);
      }
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

#if BLOG20201111
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
#if 0
  lua_getfield(L, -1, "animal");
  if (LUA_TTABLE == lua_type(L, -1)) {
    printf("DUMP: { animal => ... }\n");
  }
  lua_pop(L, 1);
  dump(L);
#endif
  lua_setglobal(L, "table");
  dump(L);
  /* can't inspect table correctly */
  luaL_dostring(L, "inspect=require('inspect'); print(inspect(table))");
#else
  /* Use lua_insert */
  /*
    {
    "cat" => "Meow"
    }
  */
  lua_pushstring(L, "cat"); /* {}, cat */
  lua_pushstring(L, "Meow"); /* {}, cat, Meow */
  lua_settable(L, -3); /* { cat = Meow } */

  /*
    animal => {
      "cat" => "Meow"
    }
  */
  lua_pushstring(L, "animal"); /* { ... }, animal */
  lua_insert(L, -2); /* animal, { ... } */

  /*
    { animal => { "cat" => "Meow" } }
   */
  lua_newtable(L); /* animal, {...}, {} */
  dump(L);
  lua_insert(L, -3); /* {}, animal, {...} */
  lua_settable(L, -3);
  dump(L);

  lua_pushstring(L, "table"); /* {}, table */
  dump(L);
  lua_insert(L, -2);
  lua_settable(L, LUA_GLOBALSINDEX);

  luaL_dostring(L, "inspect=require('inspect');print(inspect(table))");

#endif
  lua_close(L);
  return 0;
}
