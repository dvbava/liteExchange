#ifndef _SERVER_FIX_H_
#define _SERVER_FIX_H_

#include <string>
#include <vector>
#include <memory>

#include <server/server_base.h>

#include <server/command_line_interface.h>
#include <server/server_configuration.h>
#include <server/server_outgoing_message_processor.h>
#include <server/server_incoming_message_dispatcher.h>
#include <server/server_error.h>

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/ThreadedSocketAcceptor.h>

// FIX 4.2 specific
#include <quickfix/fix42//NewOrderSingle.h>
#include <quickfix/fix42/OrderCancelRequest.h>

class ServerFix : public ServerBase, public FIX::Application, public FIX::MessageCracker
{
    public:
        ServerFix(const std::string& fixEngineConfigFile, const ServerConfiguration& serverConfiguration);
        ~ServerFix();
        void run() override;
        order_matcher::CentralOrderBook& getCentralOrderBook() { return m_centralOrderBook;  }

    private:
        // FIX Engine Application overloads
        void onCreate(const FIX::SessionID&) override;
        void toAdmin(FIX::Message&, const FIX::SessionID&) override;
        void fromAdmin(const FIX::Message&, const FIX::SessionID&) throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) override;
        void onLogon(const FIX::SessionID& sessionID) override;
        void onLogout(const FIX::SessionID& sessionID) override;
        void toApp(FIX::Message&, const FIX::SessionID&) throw(FIX::DoNotSend) override;
        void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType) override;

        // FIX Engine MessageCracker overloads
        void onMessage(const FIX42::NewOrderSingle&, const FIX::SessionID&);
        void onMessage(const FIX42::OrderCancelRequest&, const FIX::SessionID&);

        std::string m_fixEngineConfigFile;
        IncomingMessageDispatcher m_dispatcher;
};

#endif