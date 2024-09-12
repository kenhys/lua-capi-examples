TARGET = settable setfield

all: $(TARGET) graph

.c: $(TARGET)
	gcc $< -I`luarocks config --lua-incdir` -L`luarocks config --lua-libdir` -llua -ldl -lm -o $@
	./$@

graph:
	dot -Tpng -osettable.png settable.dot
	dot -Tpng -osetfield.png setfield.dot

clean:
	rm -f $(TARGET)
