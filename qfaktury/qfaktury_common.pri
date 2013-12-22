QT += sql xml network widgets printsupport

# The application version
VERSION = 0.8.2
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QMAKE_CXXFLAGS += -Wall

LIBS += -lgmpxx -lgmp
