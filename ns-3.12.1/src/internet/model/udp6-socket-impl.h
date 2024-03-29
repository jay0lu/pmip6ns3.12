/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * UDP6 Implementation
 *
 * Copyright (c) 2010 KUT, ETRI
 * (Korea Univerity of Technology and Education)
 * (Electronics and Telecommunications Research Institute)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Hyon-Young Choi <commani@gmail.com>
 */
 
#ifndef UDP6_SOCKET_IMPL_H
#define UDP6_SOCKET_IMPL_H

#include <stdint.h>
#include <queue>
#include "ns3/callback.h"
#include "ns3/traced-callback.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/ipv6-address.h"
#include "ns3/udp-socket.h"
#include "ns3/ipv6-interface.h"

namespace ns3 {

class Ipv6EndPoint;
class Node;
class Packet;
class Udp6L4Protocol;

/**
 * \ingroup udp6
 * \brief A sockets interface to UDPv6
 * 
 * This class subclasses ns3::Udp6Socket, and provides a socket interface
 * to ns3's implementation of UDPv6.
 */

class Udp6SocketImpl : public UdpSocket
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound udp socket.
   */
  Udp6SocketImpl ();
  virtual ~Udp6SocketImpl ();

  void SetNode (Ptr<Node> node);
  void SetUdp (Ptr<Udp6L4Protocol> udp);

  virtual enum SocketErrno GetErrno (void) const;
  virtual enum SocketType GetSocketType (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address &address);
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect (const Address &address);
  virtual int Listen (void);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Send (Ptr<Packet> p, uint32_t flags);
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address &address);
  virtual uint32_t GetRxAvailable (void) const;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags,
                                Address &fromAddress);
  virtual int GetSockName (Address &address) const; 
  virtual int MulticastJoinGroup (uint32_t interfaceIndex, const Address &groupAddress);
  virtual int MulticastLeaveGroup (uint32_t interfaceIndex, const Address &groupAddress);
  virtual void BindToNetDevice (Ptr<NetDevice> netdevice);
  virtual bool SetAllowBroadcast (bool allowBroadcast);
  virtual bool GetAllowBroadcast () const;

private:
  // Attributes set through UdpSocket base class 
  virtual void SetRcvBufSize (uint32_t size);
  virtual uint32_t GetRcvBufSize (void) const;
  virtual void SetIpTtl (uint8_t ipTtl);
  virtual uint8_t GetIpTtl (void) const;
  virtual void SetIpMulticastTtl (uint8_t ipTtl);
  virtual uint8_t GetIpMulticastTtl (void) const;
  virtual void SetIpMulticastIf (int32_t ipIf);
  virtual int32_t GetIpMulticastIf (void) const;
  virtual void SetIpMulticastLoop (bool loop);
  virtual bool GetIpMulticastLoop (void) const;
  virtual void SetMtuDiscover (bool discover);
  virtual bool GetMtuDiscover (void) const;


  friend class Udp6SocketFactory;
  // invoked by Udp class
  int FinishBind (void);
  void ForwardUp (Ptr<Packet> packet, Ipv6Address saddr, Ipv6Address daddr, 
                  uint16_t sport, uint16_t dport, Ptr<Ipv6Interface> incomingInterface);
  void Destroy (void);
  int DoSend (Ptr<Packet> p);
  int DoSendTo (Ptr<Packet> p, const Address &daddr);
  int DoSendTo (Ptr<Packet> p, Ipv6Address daddr, uint16_t dport);
  void ForwardIcmp (Ipv6Address icmpSource, uint8_t icmpTtl, 
                    uint8_t icmpType, uint8_t icmpCode,
                    uint32_t icmpInfo);

  Ipv6EndPoint *m_endPoint;
  Ptr<Node> m_node;
  Ptr<Udp6L4Protocol> m_udp;
  Ipv6Address m_defaultAddress;
  uint16_t m_defaultPort;
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  bool m_connected;
  bool m_allowBroadcast;

  std::queue<Ptr<Packet> > m_deliveryQueue;
  uint32_t m_rxAvailable;

  // Socket attributes
  uint32_t m_rcvBufSize;
  uint8_t m_ipTtl;
  uint8_t m_ipMulticastTtl;
  int32_t m_ipMulticastIf;
  bool m_ipMulticastLoop;
  bool m_mtuDiscover;
  Callback<void, Ipv6Address,uint8_t,uint8_t,uint8_t,uint32_t> m_icmpCallback;
};

} //namespace ns3

#endif /* UDP6_SOCKET_IMPL_H */
