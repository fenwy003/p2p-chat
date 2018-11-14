#include "message.h"

Message::Message(QString sender, QDateTime timestamp) : _sender(sender), _timestamp(timestamp) {
}

Message::~Message() {
}

QString Message::sender() const {
  return _sender;
}

bool Message::isEmpty() const {
  return _sender.isEmpty();
}

QDateTime Message::timestamp() const {
  return _timestamp;
}
