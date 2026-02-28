THIRD_PARTY  = ./third_party
DIR_C_JSON   = ${THIRD_PARTY}/c_json
DIR_OUTPUT   = output_handler
DIR_CURRENT  = current_handler
DIR_I2C      = i2c_handler
DIR_MQTT     = mqtt_handler
DIR_GPIO     = gpio_handler
DIR_BIN      = ./bin

TARGET = main
OUT = greenhouse_controller

OBJ_C = $(wildcard ${DIR_C_JSON}/*.c ${DIR_OUTPUT}/*.c ${DIR_CURRENT}/*.c ${DIR_I2C}/*.c ${DIR_MQTT}/*.c ${DIR_GPIO}/*.c) ${TARGET}.c
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

INCLUDES= -I$(DIR_C_JSON) -I$(DIR_OUTPUT) -I$(DIR_CURRENT) -I$(DIR_I2C) -I$(DIR_MQTT) -I$(DIR_GPIO) -I.

LIB = -lm -lmosquitto -lgpiod

CC = gcc
MSG = -g -O0 -Wall
CFLAGS += $(MSG) $(DEBUG) -MMD -MP

${TARGET}: $(OBJ_O)
	$(MKDIR)
	$(CC) $(CFLAGS) $(OBJ_O) -o ${OUT} $(LIB)

${DIR_BIN}/main.o: main.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_C_JSON)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(INCLUDES)

${DIR_BIN}/%.o:$(DIR_OUTPUT)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(INCLUDES)

${DIR_BIN}/%.o: ${DIR_CURRENT}/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_MQTT}/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_I2C}/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

${DIR_BIN}/%.o: ${DIR_GPIO}/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(DIR_BIN)/*.o $(TARGET)

-include $(OBJ_O:.o=.d)
