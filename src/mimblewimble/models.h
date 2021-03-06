// Copyright(C) 2011 - 2020 The Litecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LITECOIN_MIMBLEWIMBLE_MODELS_H
#define LITECOIN_MIMBLEWIMBLE_MODELS_H

#include <serialize.h>
#include <libmw/libmw.h>
#include <vector>
#include <memory>

struct CMWBlock
{
    using CPtr = std::shared_ptr<CMWBlock>;

    libmw::BlockRef m_block;

    CMWBlock() = default;
    CMWBlock(const libmw::BlockRef& block)
        : m_block(block) { }

    uint64_t GetTotalFee() const noexcept
    {
        return IsNull() ? 0 : m_block.GetTotalFee();
    }

    libmw::HeaderRef GetMWEBHeader() const noexcept
    {
        return IsNull() ? libmw::HeaderRef{} : m_block.GetHeader();
    }

    std::set<libmw::KernelHash> GetKernelHashes() const
    {
        return IsNull() ? std::set<libmw::KernelHash>{} : m_block.GetKernelHashes();
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
    {
        if (ser_action.ForRead()) {
            // Deserialize
            std::vector<uint8_t> bytes;
            READWRITE(bytes);

            if (!bytes.empty()) {
                m_block = libmw::DeserializeBlock(bytes);
            }
        } else {
            // Serialize
            if (!IsNull()) {
                std::vector<uint8_t> bytes = libmw::SerializeBlock(m_block);
                READWRITE(bytes);
            } else {
                READWRITE(std::vector<uint8_t>{});
            }
        }
    }

    bool IsNull() const noexcept { return m_block.pBlock == nullptr; }
    void SetNull() noexcept { m_block = libmw::BlockRef{ nullptr }; }
};

struct CMWTx
{
    using CPtr = std::shared_ptr<CMWTx>;

    libmw::TxRef m_transaction;

    // MW: TODO - Cache kernel hashes and input/output commits

    CMWTx() = default;
    CMWTx(const libmw::TxRef& tx)
        : m_transaction(tx) { }

    std::set<libmw::KernelHash> GetKernelHashes() const
    {
        return IsNull() ? std::set<libmw::KernelHash>{} : m_transaction.GetKernelHashes();
    }

    std::set<libmw::Commitment> GetInputCommits() const noexcept
    {
        return IsNull() ? std::set<libmw::Commitment>{} : m_transaction.GetInputCommits();
    }

    std::set<libmw::Commitment> GetOutputCommits() const noexcept
    {
        return IsNull() ? std::set<libmw::Commitment>{} : m_transaction.GetOutputCommits();
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
    {
        if (ser_action.ForRead()) {
            // Deserialize
            std::vector<uint8_t> bytes;
            READWRITE(bytes);

            if (!bytes.empty()) {
                m_transaction = libmw::DeserializeTx(bytes);
            }
        } else {
            // Serialize
            if (!IsNull()) {
                std::vector<uint8_t> bytes = libmw::SerializeTx(m_transaction);
                READWRITE(bytes);
            } else {
                READWRITE(std::vector<uint8_t>{});
            }
        }
    }

    bool IsNull() const noexcept { return m_transaction.pTransaction == nullptr; }
    void SetNull() noexcept { m_transaction.pTransaction = nullptr; }
};

#endif // LITECOIN_MIMBLEWIMBLE_MODELS_H