QT += sql xml network

# The application version
VERSION = 0.8.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QMAKE_CXXFLAGS += -Wall

LIBS += -lqdecimal -ldecnumber
