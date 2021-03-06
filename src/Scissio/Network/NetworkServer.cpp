#include "NetworkServer.hpp"
#include "../Utils/Log.hpp"

using namespace Scissio;

Network::Server::Server() {
    startIoService();
}

Network::Server::~Server() {
    try {
        stopIoService();
    } catch (std::exception& e) {
        Log::e("Network client stop error: {}", e.what());
    }
}

void Network::Server::startIoService() {
    work = std::make_unique<asio::io_service::work>(service);
    Log::i("Server network asio service started!");
    thread = std::thread([this]() {
        service.run();
        Log::w("Server network asio service stopped!");
    });
}

void Network::Server::stopIoService() {
    if (thread.joinable()) {
        work.reset();
        {
            std::unique_lock<std::shared_mutex> lock{mutex};
            for (auto& pair : sessions) {
                if (const auto session = pair.second.lock()) {
                    session->disconnect();
                }
            }
        }
        service.stop();
        thread.join();
    }
}

void Network::Server::receive(const StreamPtr& stream, const Packet& packet) {
    switch (packet.id) {
    case MessageSessionInit::KIND: {
        std::unique_lock<std::shared_mutex> lock{mutex};

        const auto message = Details::unpack<MessageSessionInit>(packet);
        auto session = createSession(message.playerId, message.password);

        Log::d("New connection accepted uid: {} session: {}", message.playerId, session->getSessionId());

        MessageSessionInitResponse response;
        response.sessionId = session->getSessionId();

        session->addStream(stream);
        stream->send(response, session->getSessionId());

        sessions.insert(std::make_pair(session->getSessionId(), std::move(session)));

        break;
    }
    case MessageSessionConnect::KIND: {
        std::unique_lock<std::shared_mutex> lock{mutex};

        auto message = Details::unpack<MessageSessionConnect>(packet);

        const auto found = sessions.find(message.sessionId);
        if (found != sessions.end()) {
            if (const auto session = found->second.lock()) {
                Log::d("New stream added to session: {}", session->getSessionId());

                session->addStream(stream);
                stream->send(message, session->getSessionId());

                Log::d("Session: {} streams: {} acceptors: {}", session->getSessionId(), session->getStreams().size(),
                       acceptors.size());
                if (session->getStreams().size() == acceptors.size()) {

                    acceptSession(session);
                }
            } else {
                Log::e("Unable to add stream to broken session: {}", message.sessionId);
            }
        } else {
            Log::e("Unable to add stream to non-existing session: {}", message.sessionId);
        }

        break;
    }
    default: {
        std::shared_lock<std::shared_mutex> lock{mutex};
        const auto it = sessions.find(packet.sessionId);
        if (it != sessions.end()) {
            if (auto session = it->second.lock()) {
                dispatch(session, packet);
            }
        }
    }
    }
}
