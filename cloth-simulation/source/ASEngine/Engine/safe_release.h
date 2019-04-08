//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SAFE_RELEASE_H_20140720
#define GUARD_AIDEN_STOREY_SAFE_RELEASE_H_20140720

#define ReleaseCOM( x ) { if( x != nullptr ) { x->Release(); x = nullptr; } }

#endif // GUARD_AIDEN_STOREY_SAFE_RELEASE_H_20140720