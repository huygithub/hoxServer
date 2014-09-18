# Your project's name.
PROGRAM = hoxServer 

SERVER_SOURCES = \
	server/*.cpp \
	server/*.h \
	server/Makefile \
	server/*.sh \
	server/*.cfg

DBAGENT_SOURCES = \
	dbagent/*.cpp \
	dbagent/*.h \
	dbagent/Makefile \
	dbagent/*.sh

zip:
	zip $(PROGRAM) $(SERVER_SOURCES) $(DBAGENT_SOURCES) Makefile

