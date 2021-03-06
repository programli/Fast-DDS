// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file EDP.h
 *
 */

#ifndef _FASTDDS_RTPS_EDP_H_
#define _FASTDDS_RTPS_EDP_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC

#include <fastdds/rtps/attributes/RTPSParticipantAttributes.h>
#include <fastdds/rtps/builtin/data/ReaderProxyData.h>
#include <fastdds/rtps/builtin/data/WriterProxyData.h>
#include <fastdds/rtps/common/Guid.h>
#include <fastdds/dds/core/status/PublicationMatchedStatus.hpp>
#include <fastdds/dds/core/status/SubscriptionMatchedStatus.hpp>

namespace eprosima {

namespace fastrtps {

namespace types {
class TypeIdentifier;
}

class TopicAttributes;

namespace rtps {

class PDP;
class ParticipantProxyData;
class RTPSWriter;
class RTPSReader;
class WriterProxyData;
class RTPSParticipantImpl;

/**
 * Class EDP, base class for Endpoint Discovery Protocols. It contains generic methods used by the two EDP implemented (EDPSimple and EDPStatic), as well as abstract methods
 * definitions required by the specific implementations.
 * @ingroup DISCOVERY_MODULE
 */
class EDP
{
public:

    /**
     * Constructor.
     * @param p Pointer to the PDPSimple
     * @param part Pointer to the RTPSParticipantImpl
     */
    EDP(
            PDP* p,
            RTPSParticipantImpl* part);
    virtual ~EDP();

    /**
     * Abstract method to initialize the EDP.
     * @param attributes DiscoveryAttributes structure.
     * @return True if correct.
     */
    virtual bool initEDP(
            BuiltinAttributes& attributes) = 0;
    /**
     * Abstract method that assigns remote endpoints when a new RTPSParticipantProxyData is discovered.
     * @param pdata Discovered ParticipantProxyData
     */
    virtual void assignRemoteEndpoints(
            const ParticipantProxyData& pdata) = 0;
    /**
     * Remove remote endpoints from the endpoint discovery protocol
     * @param pdata Pointer to the ParticipantProxyData to remove
     */
    virtual void removeRemoteEndpoints(
            ParticipantProxyData* pdata)
    {
        (void) pdata;
    }

    //! Verify if the given participant EDP enpoints are matched with us
    virtual bool areRemoteEndpointsMatched(
            const ParticipantProxyData* )
    {
        return false;
    }

    /**
     * Abstract method that removes a local Reader from the discovery method
     * @param R Pointer to the Reader to remove.
     * @return True if correctly removed.
     */
    virtual bool removeLocalReader(
            RTPSReader* R) = 0;
    /**
     * Abstract method that removes a local Writer from the discovery method
     * @param W Pointer to the Writer to remove.
     * @return True if correctly removed.
     */
    virtual bool removeLocalWriter(
            RTPSWriter* W) = 0;

    /**
     * After a new local ReaderProxyData has been created some processing is needed (depends on the implementation).
     * @param reader Pointer to the Reader object.
     * @param rdata Pointer to the ReaderProxyData object.
     * @return True if correct.
     */
    virtual bool processLocalReaderProxyData(
            RTPSReader* reader,
            ReaderProxyData* rdata) = 0;

    /**
     * After a new local WriterProxyData has been created some processing is needed (depends on the implementation).
     * @param writer Pointer to the Writer object.
     * @param wdata Pointer to the Writer ProxyData object.
     * @return True if correct.
     */
    virtual bool processLocalWriterProxyData(
            RTPSWriter* writer,
            WriterProxyData* wdata) = 0;

    /**
     * Create a new ReaderPD for a local Reader.
     * @param R Pointer to the RTPSReader.
     * @param att Attributes of the associated topic
     * @param qos QoS policies dictated by the subscriber
     * @return True if correct.
     */
    bool newLocalReaderProxyData(
            RTPSReader* R,
            const TopicAttributes& att,
            const ReaderQos& qos);
    /**
     * Create a new ReaderPD for a local Writer.
     * @param W Pointer to the RTPSWriter.
     * @param att Attributes of the associated topic
     * @param qos QoS policies dictated by the publisher
     * @return True if correct.
     */
    bool newLocalWriterProxyData(
            RTPSWriter* W,
            const TopicAttributes& att,
            const WriterQos& qos);
    /**
     * A previously created Reader has been updated
     * @param R Pointer to the reader;
     * @param att Attributes of the associated topic
     * @param qos QoS policies dictated by the subscriber
     * @return True if correctly updated
     */
    bool updatedLocalReader(
            RTPSReader* R,
            const TopicAttributes& att,
            const ReaderQos& qos);
    /**
     * A previously created Writer has been updated
     * @param W Pointer to the Writer
     * @param att Attributes of the associated topic
     * @param qos QoS policies dictated by the publisher
     * @return True if correctly updated
     */
    bool updatedLocalWriter(
            RTPSWriter* W,
            const TopicAttributes& att,
            const WriterQos& qos);
    /**
     * Check the validity of a matching between a RTPSWriter and a ReaderProxyData object.
     * @param wdata Pointer to the WriterProxyData object.
     * @param rdata Pointer to the ReaderProxyData object.
     * @return True if the two can be matched.
     */
    bool validMatching(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata);
    /**
     * Check the validity of a matching between a RTPSReader and a WriterProxyData object.
     * @param rdata Pointer to the ReaderProxyData object.
     * @param wdata Pointer to the WriterProxyData object.
     * @return True if the two can be matched.
     */
    bool validMatching(
            const ReaderProxyData* rdata,
            const WriterProxyData* wdata);

    /**
     * Unpair a WriterProxyData object from all local readers.
     * @param participant_guid GUID of the participant.
     * @param writer_guid GUID of the writer.
     * @return True if correct.
     */
    bool unpairWriterProxy(
            const GUID_t& participant_guid,
            const GUID_t& writer_guid);

    /**
     * Unpair a ReaderProxyData object from all local writers.
     * @param participant_guid GUID of the participant.
     * @param reader_guid GUID of the reader.
     * @return True if correct.
     */
    bool unpairReaderProxy(
            const GUID_t& participant_guid,
            const GUID_t& reader_guid);

    /**
     * Try to pair/unpair ReaderProxyData.
     * @param participant_guid Identifier of the participant.
     * @param rdata Pointer to the ReaderProxyData object.
     * @return True.
     */
    bool pairing_reader_proxy_with_any_local_writer(
            const GUID_t& participant_guid,
            ReaderProxyData* rdata);

#if HAVE_SECURITY
    bool pairing_reader_proxy_with_local_writer(
            const GUID_t& local_writer,
            const GUID_t& remote_participant_guid,
            ReaderProxyData& rdata);

    bool pairing_remote_reader_with_local_writer_after_security(
            const GUID_t& local_writer,
            const ReaderProxyData& remote_reader_data);
#endif

    /**
     * Try to pair/unpair WriterProxyData.
     * @param participant_guid Identifier of the participant.
     * @param wdata Pointer to the WriterProxyData.
     * @return True.
     */
    bool pairing_writer_proxy_with_any_local_reader(
            const GUID_t& participant_guid,
            WriterProxyData* wdata);

#if HAVE_SECURITY
    bool pairing_writer_proxy_with_local_reader(
            const GUID_t& local_reader,
            const GUID_t& remote_participant_guid,
            WriterProxyData& wdata);

    bool pairing_remote_writer_with_local_reader_after_security(
            const GUID_t& local_reader,
            const WriterProxyData& remote_writer_data);

    virtual bool pairing_remote_writer_with_local_builtin_reader_after_security(
            const GUID_t& /*local_reader*/,
            const WriterProxyData& /*remote_writer_data*/)
    {
        return false;
    }

    virtual bool pairing_remote_reader_with_local_builtin_writer_after_security(
            const GUID_t& /*local_writer*/,
            const ReaderProxyData& /*remote_reader_data*/)
    {
        return false;
    }

#endif
    const fastdds::dds::SubscriptionMatchedStatus& update_subscription_matched_status(
            const GUID_t& reader_guid,
            const GUID_t& writer_guid,
            int change);

    const fastdds::dds::PublicationMatchedStatus& update_publication_matched_status(
            const GUID_t& reader_guid,
            const GUID_t& writer_guid,
            int change);

    //! Pointer to the PDP object that contains the endpoint discovery protocol.
    PDP* mp_PDP;
    //! Pointer to the RTPSParticipant.
    RTPSParticipantImpl* mp_RTPSParticipant;

private:

    /**
     * Try to pair/unpair a local Reader against all possible writerProxy Data.
     * @param R Pointer to the Reader
     * @param participant_guid
     * @param rdata
     * @return True
     */
    bool pairingReader(
            RTPSReader* R,
            const GUID_t& participant_guid,
            const ReaderProxyData& rdata);
    /**
     * Try to pair/unpair a local Writer against all possible readerProxy Data.
     * @param W Pointer to the Writer
     * @param participant_guid
     * @param wdata
     * @return True
     */
    bool pairingWriter(
            RTPSWriter* W,
            const GUID_t& participant_guid,
            const WriterProxyData& wdata);

    bool checkDataRepresentationQos(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    bool checkTypeValidation(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    bool checkTypeIdentifier(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    bool hasTypeIdentifier(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    bool checkTypeObject(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    bool hasTypeObject(
            const WriterProxyData* wdata,
            const ReaderProxyData* rdata) const;

    ReaderProxyData temp_reader_proxy_data_;
    WriterProxyData temp_writer_proxy_data_;
    std::map<GUID_t, fastdds::dds::SubscriptionMatchedStatus> reader_status_;
    std::map<GUID_t, fastdds::dds::PublicationMatchedStatus> writer_status_;
};

} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */

#endif
#endif /* _FASTDDS_RTPS_EDP_H_ */
