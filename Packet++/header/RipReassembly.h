#ifndef PACKETPP_RIP_REASSEMBLY
#define PACKETPP_RIP_REASSEMBLY

#include "IpAddress.h"
#include "Packet.h"
#include <map>


/**
 * @namespace pcpp
 * @brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{
class RipPacketData
{
  public:
	RipPacketData(const uint8_t *ripData, size_t ripDataLength, std::string tupleName)
		: m_Data(ripData), m_DataLen(ripDataLength), m_TupleName(tupleName)
	{
	}

	const uint8_t *getData() const
	{
		return m_Data;
	}

	size_t getDataLength() const
	{
		return m_DataLen;
	}

	std::string getTupleName()
	{
		return m_TupleName;
	}

  private:
	const uint8_t *m_Data;
	size_t m_DataLen;
	std::string m_TupleName;
};

class RIPReassembly
{
  public:
	/**
	 * @typedef OnRipMessageReady
	 * A callback invoked when new data arrives
	 */
	typedef void (*OnRipMessageReady)(pcpp::RipPacketData *ripData, void *userCookie);

	/**
	 * An enum representing the status returned from processing a fragment
	 */
	enum ReassemblyStatus
	{
		NonIpPacket,
		NonUdpPacket,
		NonRipPacket,
		RipMessageHandled,
	};

	RIPReassembly(OnRipMessageReady onRipMessageReadyCallback, void *callbackUserCookie = NULL)
		: m_OnRipMessageReadyCallback(onRipMessageReadyCallback), m_CallbackUserCookie(callbackUserCookie)
	{
	}


	ReassemblyStatus reassemblePacket(Packet &ripData);

	ReassemblyStatus reassemblePacket(RawPacket *ripRawData);

	std::string getTupleName(IPAddress src, IPAddress dst, uint16_t srcPort, uint16_t dstPort);

  private:
	struct RIPReassemblyData
	{
		IPAddress srcIP;
		IPAddress dstIP;
		uint16_t srcPort;
		uint16_t dstPort;
		std::string tupleName;
		uint16_t number;

		RIPReassemblyData()
		{
		}
		RIPReassemblyData(IPAddress src, IPAddress dst, uint16_t srcP, uint16_t dstP, std::string tName, uint16_t n)
			: srcIP(src), dstIP(dst), srcPort(srcP), dstPort(dstP), tupleName(tName), number(n)
		{
		}
	};

	typedef std::map<std::string, RIPReassemblyData> FragmentList;

	FragmentList m_FragmentList;
	OnRipMessageReady m_OnRipMessageReadyCallback;
	void *m_CallbackUserCookie;
};

} // namespace pcpp

#endif // PACKETPP_RIP_REASSEMBLY
