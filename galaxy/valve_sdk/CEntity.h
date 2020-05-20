#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "CInput.h"

#include "..\valve_utils\Utils.h"
#include "IVModelInfo.h"
#include "..\valve_utils\NetvarManager.h"

#define OFFSET( type, offset )		( *( type* ) ( ( std::uintptr_t ) this + ( offset ) ) )
#define OFFSETPTR( type, offset )    ( ( type ) ( ( std::uintptr_t ) this + ( offset ) ) )
#define FOFFSET( type, ptr, offset ) ( ( type ) ( ( std::uintptr_t ) (ptr) + ( offset ) ) )
#define Assert( _exp ) ((void)0)
#define offset(func, type, offset) type& func { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

#define member_func_args(...) (this, __VA_ARGS__ ); }
#define vfunc(index, func, sig) auto func { return reinterpret_cast<sig>((*(uint32_t**)this)[index]) member_func_args
class IKContext
{
public:
	void init( studiohdr_t* hdr, Vector& angles, Vector& origin, float curtime, int framecount, int boneMask );
	void update_targets( Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed );
	void solve_dependencies( Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed );

	void clear_targets( )
	{
		
		auto v56 = 0;
		if (*(int*)((DWORD)this + 4080) > 0)
		{
			auto v57 = (int*)((DWORD)this + 208);
			do
			{
				*v57 = -9999;
				v57 += 85;
				++v56;
			} while (v56 < *(int*)((DWORD)this + 4080));
		}
	}
};


class CBasePlayerAnimState
{
public:
	char pad[4];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
	float& m_flAbsRotation( ) {
		return *(float*)((uintptr_t)this + 0x80);
	}
};


class CBoneAccessor
{

public:

	inline matrix3x4_t* GetBoneArrayForWrite( )
	{
		return m_pBones;
	}

	inline void SetBoneArrayForWrite( matrix3x4_t* bone_array )
	{
		m_pBones = bone_array;
	}

	alignas(16) matrix3x4_t* m_pBones;
	int32_t m_ReadableBones; // Which bones can be read.
	int32_t m_WritableBones; // Which bones can be written.
}; 

struct _CrtMemState;

#define MEMALLOC_VERSION 1

typedef size_t( *MemAllocFailHandler_t ) (size_t);

//-----------------------------------------------------------------------------
// NOTE! This should never be called directly from leaf code
// Just use new,delete,malloc,free etc. They will call into this eventually
//-----------------------------------------------------------------------------
class IMemAlloc
{
public:
	virtual ~IMemAlloc( );

	// Release versions
	virtual void* Alloc( size_t nSize ) = 0;
	virtual void* Realloc( void* pMem, size_t nSize ) = 0;
	virtual void Free( void* pMem ) = 0;
	virtual void* Expand_NoLongerSupported( void* pMem, size_t nSize ) = 0;

	// Debug versions
	//virtual void *Alloc(size_t nSize, const char *pFileName, int nLine) = 0;
	//virtual void *Realloc(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;
	//virtual void  Free(void *pMem, const char *pFileName, int nLine) = 0;
	//virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize, const char *pFileName, int nLine) = 0;

	// Returns size of a particular allocation
	virtual size_t GetSize( void* pMem ) = 0;

	// Force file + line information for an allocation
	virtual void PushAllocDbgInfo( const char* pFileName, int nLine ) = 0;
	virtual void PopAllocDbgInfo( ) = 0;

	// FIXME: Remove when we have our own allocator
	// these methods of the Crt debug code is used in our codebase currently
	virtual long CrtSetBreakAlloc( long lNewBreakAlloc ) = 0;
	virtual int CrtSetReportMode( int nReportType, int nReportMode ) = 0;
	virtual int CrtIsValidHeapPointer( const void* pMem ) = 0;
	virtual int CrtIsValidPointer( const void* pMem, unsigned int size, int access ) = 0;
	virtual int CrtCheckMemory( void ) = 0;
	virtual int CrtSetDbgFlag( int nNewFlag ) = 0;
	virtual void CrtMemCheckpoint( _CrtMemState* pState ) = 0;

	// FIXME: Make a better stats interface
	virtual void DumpStats( ) = 0;
	virtual void DumpStatsFileBase( char const* pchFileBase ) = 0;

	// FIXME: Remove when we have our own allocator
	virtual void* CrtSetReportFile( int nRptType, void* hFile ) = 0;
	virtual void* CrtSetReportHook( void* pfnNewHook ) = 0;
	virtual int CrtDbgReport( int nRptType, const char* szFile,
		int nLine, const char* szModule, const char* pMsg ) = 0;

	virtual int heapchk( ) = 0;

	virtual bool IsDebugHeap( ) = 0;

	virtual void GetActualDbgInfo( const char*& pFileName, int& nLine ) = 0;
	virtual void RegisterAllocation( const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) = 0;
	virtual void RegisterDeallocation( const char* pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) = 0;

	virtual int GetVersion( ) = 0;

	virtual void CompactHeap( ) = 0;

	// Function called when malloc fails or memory limits hit to attempt to free up memory (can come in any thread)
	virtual MemAllocFailHandler_t SetAllocFailHandler( MemAllocFailHandler_t pfnMemAllocFailHandler ) = 0;

	virtual void DumpBlockStats( void* ) = 0;

#if defined( _MEMTEST )
	virtual void SetStatsExtraInfo( const char* pMapName, const char* pComment ) = 0;
#endif

	// Returns 0 if no failure, otherwise the size_t of the last requested chunk
	//  "I'm sure this is completely thread safe!" Brian Deen 7/19/2012.
	virtual size_t MemoryAllocFailed( ) = 0;

	// handles storing allocation info for coroutines
	virtual int GetDebugInfoSize( ) = 0;
	virtual void SaveDebugInfo( void* pvDebugInfo ) = 0;
	virtual void RestoreDebugInfo( const void* pvDebugInfo ) = 0;
	virtual void InitDebugInfo( void* pvDebugInfo, const char* pchRootFileName, int nLine ) = 0;

	// Replacement for ::GlobalMemoryStatus which accounts for unused memory in our system
	virtual void GlobalMemoryStatus( size_t* pUsedMemory, size_t* pFreeMemory ) = 0;
}; extern IMemAlloc* csgo_mem_alloc;


// class predefinition
class C_BaseCombatWeapon;

class C_AnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};

class AnimationLayer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038


class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:

	C_AnimState *AnimState()
	{
		return *reinterpret_cast<C_AnimState**>(uintptr_t(this) + 0x3900);
	}

	AnimationLayer *AnimOverlays()
	{
		return *reinterpret_cast<AnimationLayer**>(uintptr_t(this) + 0x2980);
	}

	int NumOverlays()
	{
		return 15;
	}

	void UpdateClientAnimation()
	{
		Utils::GetVFunc<void(__thiscall*)(void*)>(this, 223)(this);
	}

	bool& ClientAnimations( )
	{
		return *reinterpret_cast<bool*>(uintptr_t( this ) + g_pNetvars->GetOffset( "DT_BaseAnimating", "m_bClientSideAnimation" ));
	}

	int GetSequence()
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		return GetValue<int>(m_nSequence);
	}

	void SetSequence(int Sequence)
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nSequence) = Sequence;
	}

	void SimulatedEveryTick(bool value)
	{
		static int m_bSimulatedEveryTick = g_pNetvars->GetOffset("DT_BaseEntity", "m_bSimulatedEveryTick");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bSimulatedEveryTick) = value;
	}

	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(Vector origin);
	
	AnimationLayer* GetAnimOverlays( )
	{
		return *(AnimationLayer**)((DWORD)this + 0x2990);
	}

	uint32_t& GetMostRecentModelBoneCounter( ) // for fake matrix
	{
		return *reinterpret_cast<uint32_t*>(uintptr_t( this ) + 0x2690);
	}
	float& GetLastBoneSetupTime( ) // for fake matrix
	{
		return *reinterpret_cast<float*>(uintptr_t( this ) + 0x2924);
	}

	void create_animation_state( CBasePlayerAnimState* state )
	{
		using create_anim_state_t = void( __thiscall* ) (CBasePlayerAnimState*, C_BaseEntity*);
		static auto create_anim_state = reinterpret_cast<create_anim_state_t> (Utils::FindSignature( "client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" ));

		if (!create_anim_state)
			return;

		create_anim_state( state, this );
	}

	float_t m_flSpawnTime( )
	{
		return *(float_t*)((uintptr_t)this + 0xA360);
	}

	void UpdateAnimationState( CBasePlayerAnimState* state, Vector angle )
	{
		static auto update_anim_state = Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" );

		if (!update_anim_state)
			return;

		__asm
		{
			push 0
		}

		__asm
		{
			mov ecx, state

			movss xmm1, dword ptr[angle + 4]
			movss xmm2, dword ptr[angle]

			call update_anim_state
		}
	}

	uintptr_t observer_target( )
	{
		static int m_hObserverTarget = g_pNetvars->GetOffset( "DT_BasePlayer", "m_hObserverTarget" );
		return GetValue<uintptr_t>( m_hObserverTarget );
	}

	std::array< float, 24 >& m_flPoseParameter( ) //24
	{
		static int _m_flPoseParameter = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_flPoseParameter" );
		return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + _m_flPoseParameter);
	}

	CBasePlayerAnimState* GetPlayerAnimState( )
	{
		return *(CBasePlayerAnimState**)((DWORD)this + 0x3900);
	}

	template<typename FuncType>
	__forceinline static FuncType CallVFunction( void* ppClass, int index )
	{
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}

	

	datamap_t* GetPredDescMap( ) {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>( this, 17 )(this);
	}
	offset( get_ik_context( ), IKContext*, 9836 + 0x4 );

	Vector& GetAbsAngles( )
	{
		if (!this)
			return Vector( );
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return CallVFunction<OriginalFn>( this, 11 )(this);
	}

	void StandardBlendingRules( studiohdr_t* hdr, Vector* pos, quaternion* q, float_t curtime, int32_t boneMask )
	{
		typedef void( __thiscall* o_StandardBlendingRules )(void*, studiohdr_t*, Vector*, quaternion*, float_t, int32_t);
		CallVFunction<o_StandardBlendingRules>( this, 205 )(this, hdr, pos, q, curtime, boneMask);
	}
	vfunc( 191, update_ik_locks( float curtime ), void( __thiscall* )(void*, float) )(curtime)
	vfunc( 192, calculate_ik_locks( float curtime ), void( __thiscall* )(void*, float) )(curtime)

	void BuildTransformations( studiohdr_t* hdr, Vector* pos, quaternion* q, const matrix3x4_t& cameraTransform, int32_t boneMask, byte* computed )
	{
		typedef void( __thiscall* o_BuildTransformations )(void*, studiohdr_t*, Vector*, quaternion*, const matrix3x4_t&, int32_t, byte*);
		CallVFunction<o_BuildTransformations>( this, 189 )(this, hdr, pos, q, cameraTransform, boneMask, computed);
	}

	


	int& get_effects( )
	{
		static unsigned int _m_iEFlags = Utils::FindInDataMap( GetPredDescMap( ), "m_fEffects" );
		return *(int*)((uintptr_t)this + _m_iEFlags);
	}

	void InvalidateBoneCache( )
	{
		static auto invalidate_bone_bache = Utils::FindSignature( "client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) + 10;

		*(std::uint32_t*) ((std::uintptr_t) this + 0x2924) = 0xFF7FFFFF;
		*(std::uint32_t*) ((std::uintptr_t) this + 0x2690) = **(std::uintptr_t**) invalidate_bone_bache - 1;
	}

	float MaxDesyncDelta( );

	offset( get_bone_cache_count( ), int, 0x2910 + 0xC )
	matrix3x4_t* get_bone_array_for_write( ) {
		return *(matrix3x4_t**)((uintptr_t)this + 0x26A8);
	}

	Vector GetAbsOrigin()
	{
		return Utils::GetVFunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}

	Vector& m_vecAbsVelocity( )
	{
		static unsigned int _m_vecAbsVelocity = Utils::FindInDataMap( GetPredDescMap( ), "m_vecAbsVelocity" );
		return *(Vector*)((uintptr_t)this + _m_vecAbsVelocity);
	}

	void SetAbsVelocity(Vector velocity);

    C_BaseCombatWeapon* GetActiveWeapon()
    {
        static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = GetValue<CBaseHandle>(m_hActiveWeapon);
        return reinterpret_cast<C_BaseCombatWeapon*>(csgo_entity_list->GetClientEntityFromHandle(weaponData));
    }
	int Money( )
	{
		static int m_ArmorValue = g_pNetvars->GetOffset( "DT_CSPlayer", "m_iAccount" );
		return GetValue<int>( m_ArmorValue );
	}
    int GetTeam()
    {
        static int m_iTeamNum = g_pNetvars->GetOffset("DT_BaseEntity", "m_iTeamNum");
        return GetValue<int>(m_iTeamNum);
    }

    EntityFlags get_flags()
    {
        static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
        return GetValue<EntityFlags>(m_fFlags);
    }

	void SetFlags(int offset)
	{
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		*reinterpret_cast<int*>(uintptr_t(this) + m_fFlags) = offset;
	}

    MoveType_t GetMoveType()
    {
        static int m_Movetype = g_pNetvars->GetOffset("DT_BaseEntity", "m_nRenderMode") + 1;
        return GetValue<MoveType_t>(m_Movetype);
    }

	uint32_t& m_iEFlags( )
	{
		static unsigned int _m_iEFlags = Utils::FindInDataMap( GetPredDescMap( ), "m_iEFlags" );
		return *(uint32_t*)((uintptr_t)this + _m_iEFlags);
	}

	float GetSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationTime);
	}

	float GetOldSimulationTime()
	{
		static int m_flOldSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 4;
		return GetValue<float>(m_flOldSimulationTime);
	}

	float GetLowerBodyYaw()
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return GetValue<float>(m_flLowerBodyYawTarget);
	}

	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}

	//new for anifix
	studiohdr_t* GetModelPtr( )
	{
		return *(studiohdr_t**)((uintptr_t)this + 0x294C);
	}

	CBoneAccessor* GetBoneAccessor( )
	{
		return (CBoneAccessor*)((uintptr_t)this + 0x26A8);
	}

	bool GetHeavyArmor()
	{
		static int m_bHasHeavyArmor = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
		return GetValue<bool>(m_bHasHeavyArmor);
	}

	int ArmorValue()
	{
		static int m_ArmorValue = g_pNetvars->GetOffset("DT_CSPlayer", "m_ArmorValue");
		return GetValue<int>(m_ArmorValue);
	}

	bool HasHelmet()
	{
		static int m_bHasHelmet = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return GetValue<bool>(m_bHasHelmet);
	}

    bool GetLifeState()
    {
        static int m_lifeState = g_pNetvars->GetOffset("DT_BasePlayer", "m_lifeState");
        return GetValue<bool>(m_lifeState);
    }

	bool IsScoped()
	{
		static int m_bIsScoped = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsScoped");
		return GetValue<bool>(m_bIsScoped);
	}

    int GetHealth()
    {
        static int m_iHealth = g_pNetvars->GetOffset("DT_BasePlayer", "m_iHealth");
        return GetValue<int>(m_iHealth);
    }

	bool IsKnifeorNade();
	bool IsKnife_fix( );

    bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

	bool IsEnemy();

    bool IsImmune()
    {
        static int m_bGunGameImmunity = g_pNetvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
        return GetValue<bool>(m_bGunGameImmunity);
    }

    int GetTickBase()
    {
        static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
        return GetValue<int>(m_nTickBase);
    }

	int GetShotsFired()
	{
		static int m_iShotsFired = g_pNetvars->GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");
		return GetValue<int>(m_iShotsFired);
	}

	void SetTickBase(int TickBase)
	{
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}

	Vector GetEyeAngles()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return GetValue<Vector>(m_angEyeAngles);
	}

	void SetEyeAngles(Vector Angle)
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_angEyeAngles) = Angle;
	}

    Vector GetOrigin()
    {
        static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
        return GetValue<Vector>(m_vecOrigin);
    }

	Vector GetOldOrigin()
	{
		static int m_vecOldOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 12;
		return GetValue<Vector>(m_vecOldOrigin);
	}

	Vector GetNetworkOrigin()
	{
		static int m_vecNetworkOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 40;
		return GetValue<Vector>(m_vecNetworkOrigin);
	}

	void SetOrigin(Vector Origin)
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecOrigin) = Origin;
	}

	Vector get_velocity()
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return GetValue<Vector>(m_vecVelocity);
	}

	void SetVelocity(Vector velocity)
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecVelocity) = velocity;
	}

	Vector GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x302C));
	}

	Vector GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3020));
	}

    Vector GetEyePosition() 
	{
		Vector ret;
		typedef void(__thiscall *OrigFn)(void *, Vector&);
		Utils::GetVFunc<OrigFn>(this, 284)(this, ret);
		return ret;
	}

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((DWORD)this + 0x318);
	}

	void SetCurrentCommand(CUserCmd *cmd)
	{
		static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}

	float FireRate();

	void FixSetupBones(matrix3x4_t *Matrix);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix);
};


class C_BaseCombatWeapon : public C_BaseEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

public:

    ItemDefinitionIndex GetItemDefinitionIndex()
    {
        static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
        return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
    }

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::GetVFunc< oInaccuracy >(this, 482)(this);
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::GetVFunc< oWeaponSpread >(this, 452)(this);
	}

	float GetAccuracyPenalty()
	{
		static int m_fAccuracyPenalty = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		return GetValue<float>(m_fAccuracyPenalty);
	}

	float GetLastShotTime()
	{
		static int m_fLastShotTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return GetValue<float>(m_fLastShotTime);
	}

	void AccuracyPenalty()
	{
		typedef void(__thiscall *OrigFn)(void *);
		return Utils::GetVFunc<OrigFn>(this, 483)(this);
	}

    float GetNextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
        return GetValue<float>(m_flNextPrimaryAttack);
    }

    int GetAmmo()
    {
        static int m_iClip1 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return GetValue<int>(m_iClip1);
    }

	int m_iPrimaryReserveAmmoCount( )
	{
		static int primary_reserve = g_pNetvars->GetOffset( "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount" );
		return GetValue<int>( primary_reserve );
	}

    WeaponInfo_t* GetCSWpnData()
    {
        return Utils::CallVFunc<460, WeaponInfo_t*>(this);
    }

	weapon_info_t* GetCSWpnData_2( )
	{
		if (!this) return nullptr;
		return Utils::CallVFunc<460, weapon_info_t*>( this );
	}

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->weapon_name);
    }
};
