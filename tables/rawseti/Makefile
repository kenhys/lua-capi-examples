TARGET = simple multiple nested

all: $(TARGET)

.c: $(TARGET)
	echo `pkg-config --libs luajit`
	echo `pkg-config --cflags luajit`
	gcc $< `pkg-config --cflags luajit` `pkg-config --libs luajit` -ldl -lm -o $@
	./$@

clean:
	rm -f $(TARGET)
