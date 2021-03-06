######################################################################
# Automatically generated by qmake (2.01a) Thu Sep 15 17:01:48 2011
######################################################################

TEMPLATE = lib
TARGET = dissent
DEPENDPATH += 
INCLUDEPATH += 
CONFIG += qt debug

# Input
LIBS += -lcryptopp
HEADERS += src/Dissent.hpp \
           src/Anonymity/Group.hpp \
           src/Anonymity/Log.hpp \
           src/Anonymity/NullRound.hpp \
           src/Anonymity/Round.hpp \
           src/Anonymity/SecureSession.hpp \
           src/Anonymity/Session.hpp \
           src/Anonymity/SessionManager.hpp \
           src/Anonymity/ShuffleRound.hpp \
           src/Crypto/AsymmetricKey.hpp \
           src/Crypto/CppHash.hpp \
           src/Crypto/CppPrivateKey.hpp \
           src/Crypto/CppPublicKey.hpp \
           src/Crypto/CppRandom.hpp \
           src/Crypto/Hash.hpp \
           src/Crypto/OnionEncryptor.hpp \
           src/Crypto/Random.hpp \
           src/Connections/Connection.hpp \
           src/Connections/ConnectionManager.hpp \
           src/Connections/ConnectionTable.hpp \
           src/Connections/Id.hpp \
           src/Messaging/Filter.hpp \
           src/Messaging/ISender.hpp \
           src/Messaging/ISink.hpp \
           src/Messaging/RpcHandler.hpp \
           src/Messaging/RpcMethod.hpp \
           src/Messaging/RpcRequest.hpp \
           src/Messaging/RpcResponse.hpp \
           src/Messaging/Source.hpp \
           src/Transports/Address.hpp \
           src/Transports/AddressException.hpp \
           src/Transports/AddressFactory.hpp \
           src/Transports/BufferAddress.hpp \
           src/Transports/BufferEdge.hpp \
           src/Transports/BufferEdgeListener.hpp \
           src/Transports/Edge.hpp \
           src/Transports/EdgeListener.hpp \
           src/Utils/Settings.hpp \
           src/Utils/Sleeper.hpp \
           src/Utils/Time.hpp \
           src/Utils/Timer.hpp \
           src/Utils/TimerCallback.hpp \
           src/Utils/TimerEvent.hpp 
SOURCES += src/Init.cpp \
           src/Anonymity/Group.cpp \
           src/Anonymity/Log.cpp \
           src/Anonymity/NullRound.cpp \
           src/Anonymity/Round.cpp \
           src/Anonymity/SecureSession.cpp \
           src/Anonymity/Session.cpp \
           src/Anonymity/SessionManager.cpp \
           src/Anonymity/ShuffleRound.cpp \
           src/Crypto/CppHash.cpp \
           src/Crypto/CppPrivateKey.cpp \
           src/Crypto/CppPublicKey.cpp \
           src/Crypto/CppRandom.cpp \
           src/Crypto/OnionEncryptor.cpp \
           src/Connections/Connection.cpp \
           src/Connections/ConnectionManager.cpp \
           src/Connections/ConnectionTable.cpp \
           src/Connections/Id.cpp \
           src/Messaging/RpcHandler.cpp \
           src/Messaging/RpcRequest.cpp \
           src/Messaging/RpcResponse.cpp \
           src/Messaging/Source.cpp \
           src/Transports/Address.cpp \
           src/Transports/AddressFactory.cpp \
           src/Transports/BufferAddress.cpp \
           src/Transports/BufferEdge.cpp \
           src/Transports/BufferEdgeListener.cpp \
           src/Transports/Edge.cpp \
           src/Transports/EdgeListener.cpp \
           src/Utils/Settings.cpp \
           src/Utils/Sleeper.cpp \
           src/Utils/Time.cpp \
           src/Utils/Timer.cpp \
           src/Utils/TimerEvent.cpp

