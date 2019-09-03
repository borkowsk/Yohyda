#ifndef TREE_CONSTS_H
#define TREE_CONSTS_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace facjata
{
///Defaults of IPC queue
const char* const MSGQ_NAME="FacjataDefMemoryPoolQ";
const unsigned    MSG_MAX_LEN=0xfff;
const unsigned    MSGQ_SIZE=1000;

///Defaults of IPC shared memory
const char* const MEM_POOL_NAME="FacjataDefMemoryPool";
const unsigned    MEM_POOL_SIZE=0xffffff;

///Value of this const mark finish of all tree processors job.
const char* const MEM_END="@T@h@e@E@n@d@";

}//namespace facjata
#endif // TREE_CONSTS_H
