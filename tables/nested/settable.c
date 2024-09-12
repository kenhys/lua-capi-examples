#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define BLOG20201111 1

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
      int count = 0;
      /* dump table */
      lua_pushnil(L);
      while (lua_next(L, i) != 0) {
        count++;
        if (count == 1) printf("{");
        printf(" (%s) %s => (%s) %s ",
               lua_typename(L, lua_type(L, -2)),
               lua_tostring(L, -2),
               lua_typename(L, lua_type(L, -1)),
               lua_tostring(L, -1));
        // pop only value, reuse key
        lua_pop(L, 1);
      }
      if (count == 0) {
        printf("{}\n");
      } else {
        printf("}\n");
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
  // do not register as table here
  lua_setglobal(L, "sample");
  dump(L);
  /* can't inspect table correctly */
  luaL_dostring(L, "inspect=require('inspect'); print(inspect(sample))");
#else
  /* Use lua_insert */
  lua_pushstring(L, "cat"); /* {}, cat */
  lua_pushstring(L, "Meow"); /* {}, cat, Meow */
  lua_settable(L, -3); /* { cat = Meow } */

  lua_pushstring(L, "animal"); /* { ... }, animal */
  lua_insert(L, -2); /* animal, { ... } */

  /*
    { animal => { "cat" => "Meow" } }
   */
  dump(L);
  lua_newtable(L); /* animal, {...}, {} */
  dump(L);
  lua_insert(L, -3); /* {}, animal, {...} */
  lua_settable(L, -3);
  dump(L);

  /* do not name 'table' */
  lua_pushstring(L, "sample"); /* {}, table */
  dump(L);
  lua_insert(L, -2); /* table, {...} */
  lua_settable(L, LUA_GLOBALSINDEX);

  luaL_dostring(L, "inspect=require('inspect');print(inspect(sample))");

#endif
  lua_close(L);
  return 0;
}
