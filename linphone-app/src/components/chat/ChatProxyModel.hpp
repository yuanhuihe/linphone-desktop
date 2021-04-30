/*
 * Copyright (c) 2010-2020 Belledonne Communications SARL.
 *
 * This file is part of linphone-desktop
 * (see https://www.linphone.org).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHAT_PROXY_MODEL_H_
#define CHAT_PROXY_MODEL_H_

#include <QSortFilterProxyModel>

#include "ChatModel.hpp"

// =============================================================================

class QWindow;

class ChatProxyModel : public QSortFilterProxyModel {
  class ChatModelFilter;

  Q_OBJECT;

  Q_PROPERTY(QString peerAddress READ getPeerAddress WRITE setPeerAddress NOTIFY peerAddressChanged);
  Q_PROPERTY(QString localAddress READ getLocalAddress WRITE setLocalAddress NOTIFY localAddressChanged);
  Q_PROPERTY(QString fullPeerAddress READ getFullPeerAddress WRITE setFullPeerAddress NOTIFY fullPeerAddressChanged);
  Q_PROPERTY(QString fullLocalAddress READ getFullLocalAddress WRITE setFullLocalAddress NOTIFY fullLocalAddressChanged);
  Q_PROPERTY(int isSecure READ getIsSecure WRITE setIsSecure NOTIFY isSecureChanged);
  Q_PROPERTY(std::shared_ptr<ChatModel> chatModel READ getChatModel WRITE setChatModel NOTIFY chatModelChanged);
  //Q_PROPERTY(bool isSecure MEMBER mIsSecure NOTIFY isSecureChanged);
  Q_PROPERTY(bool isRemoteComposing READ getIsRemoteComposing NOTIFY isRemoteComposingChanged);
  //Q_PROPERTY(bool isSecure READ getIsSecure NOTIFY isSecureChanged);
  Q_PROPERTY(QString cachedText READ getCachedText);

public:
  ChatProxyModel (QObject *parent = Q_NULLPTR);

  Q_INVOKABLE void loadMoreEntries ();
  Q_INVOKABLE void setEntryTypeFilter (ChatModel::EntryType type);
  Q_INVOKABLE void removeEntry (int id);

  Q_INVOKABLE void removeAllEntries ();

  Q_INVOKABLE void sendMessage (const QString &message);
  Q_INVOKABLE void resendMessage (int id);

  Q_INVOKABLE void sendFileMessage (const QString &path);

  Q_INVOKABLE void downloadFile (int id);
  Q_INVOKABLE void openFile (int id);
  Q_INVOKABLE void openFileDirectory (int id);

  Q_INVOKABLE void compose (const QString& text);

  Q_INVOKABLE void resetMessageCount();
  
signals:
  void peerAddressChanged (const QString &peerAddress);
  void localAddressChanged (const QString &localAddress);
  void fullPeerAddressChanged (const QString &fullPeerAddress);
  void fullLocalAddressChanged (const QString &fullLocalAddress);
  bool isRemoteComposingChanged (bool status);
  bool isSecureChanged(bool secure);
  
  void chatModelChanged();

  void moreEntriesLoaded (int n);

  void entryTypeFilterChanged (ChatModel::EntryType type);

protected:
  bool filterAcceptsRow (int sourceRow, const QModelIndex &sourceParent) const override;

private:
  QString getPeerAddress () const;
  void setPeerAddress (const QString &peerAddress);

  QString getLocalAddress () const;
  void setLocalAddress (const QString &localAddress);
  
  QString getFullPeerAddress () const;
  void setFullPeerAddress (const QString &peerAddress);

  QString getFullLocalAddress () const;
  void setFullLocalAddress (const QString &localAddress);
  
  int getIsSecure () const;
  void setIsSecure (const int &secure);
  
  std::shared_ptr<ChatModel> getChatModel() const;
  void setChatModel (std::shared_ptr<ChatModel> chatModel);

  bool getIsRemoteComposing () const;
  
  QString getCachedText() const;

  void reload ();

  void handleIsActiveChanged (QWindow *window);

  void handleIsRemoteComposingChanged (bool status);
  void handleMessageReceived (const std::shared_ptr<linphone::ChatMessage> &message);
  void handleMessageSent (const std::shared_ptr<linphone::ChatMessage> &message);

  int mMaxDisplayedEntries = EntriesChunkSize;

  QString mPeerAddress;
  QString mLocalAddress;
  QString mFullPeerAddress;
  QString mFullLocalAddress;
  int mIsSecure;
  static QString gCachedText;
  std::shared_ptr<linphone::ChatRoom> mChatRoom;
  

  std::shared_ptr<ChatModel> mChatModel;

  static constexpr int EntriesChunkSize = 50;
};

#endif // CHAT_PROXY_MODEL_H_
