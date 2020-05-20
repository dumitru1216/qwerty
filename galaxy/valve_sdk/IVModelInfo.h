#pragma once

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32


#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10


struct mstudiobbox_t
{
	int		bone;				// 0x0000 
	int		group;				// 0x0004 
	Vector	min;				// 0x0008 
	Vector	max;				// 0x0014 
	int		nameidx;			// 0x0020 
	char	pad_0x0024[0xC];	// 0x0024
	float	radius;				// 0x0030 
	char	pad_0x0034[0x10];	// 0x0034
};
struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              GetName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   GetHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};
struct mstudiobone_t
{
	int                                     sznameindex;
	inline char * const GetName(void) const { return ((char *)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector                          pos;
	float                           quat[4];
	Vector                          rot;
	Vector                          posscale;
	Vector                          rotscale;

	matrix3x4_t                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void *           GetProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((BYTE *)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};
struct studiohdr_t
{
	int                                     id;
	int                                     version;

	int                                     checksum;

	char                            name[64];
	int                                     length;


	Vector                          eyeposition;

	Vector                          illumposition;

	Vector                          hull_min;
	Vector                          hull_max;

	Vector                          view_bbmin;
	Vector                          view_bbmax;

	int                                     flags;

	int                                     numbones;
	int                                     boneindex;

	inline mstudiobone_t *GetBone(int i) const { return (mstudiobone_t *)(((BYTE *)this) + boneindex) + i; };
	//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int                                     numbonecontrollers;
	int                                     bonecontrollerindex;

	int                                     numhitboxsets;
	int                                     hitboxsetindex;

	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((BYTE*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* GetHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	}

	inline int GetHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int                                     numlocalanim;
	int                                     localanimindex;

	int                                     numlocalseq;
	int                                     localseqindex;

	mutable int                     activitylistversion;
	mutable int                     eventsindexed;

	int                                     numtextures;
	int                                     textureindex;

	int                                     numcdtextures;
	int                                     cdtextureindex;

	int                                     numskinref;
	int                                     numskinfamilies;
	int                                     skinindex;

	int                                     numbodyparts;
	int                                     bodypartindex;

	int                                     numlocalattachments;
	int                                     localattachmentindex;

	int                                     numlocalnodes;
	int                                     localnodeindex;
	int                                     localnodenameindex;

	int                                     numflexdesc;
	int                                     flexdescindex;

	int                                     numflexcontrollers;
	int                                     flexcontrollerindex;

	int                                     numflexrules;
	int                                     flexruleindex;

	int                                     numikchains;
	int                                     ikchainindex;

	int                                     nummouths;
	int                                     mouthindex;

	int                                     numlocalposeparameters;
	int                                     localposeparamindex;

	int                                     surfacepropindex;

	int                                     keyvalueindex;
	int                                     keyvaluesize;


	int                                     numlocalikautoplaylocks;
	int                                     localikautoplaylockindex;

	float                           mass;
	int                                     contents;

	int                                     numincludemodels;
	int                                     includemodelindex;

	mutable void            *virtualModel;

	int                                     szanimblocknameindex;
	int                                     numanimblocks;
	int                                     animblockindex;

	mutable void            *animblockModel;

	int                                     bonetablebynameindex;

	void                            *pVertexBase;
	void                            *pIndexBase;

	BYTE                            constdirectionallightdot;

	BYTE                            rootLOD;

	BYTE                            numAllowedRootLODs;

	BYTE                            unused[1];

	int                                     unused4;

	int                                     numflexcontrollerui;
	int                                     flexcontrolleruiindex;
	float                           flVertAnimFixedPointScale;
	int                                     unused3[1];
	int                                     studiohdr2index;
	int                                     unused2[1];
};

struct model_t;

class IVModelInfo // gucci
{
public:
	inline void* GetModel(int Index) {
		using Fn = void*(__thiscall*)(IVModelInfo*, int);
		return (*(Fn**)this)[1](this, Index);
	}
	int	GetModelIndex(const char *name)
	{
		using Fn = int(__thiscall*)(IVModelInfo*, const char*);
		return (*(Fn**)this)[2](this, name);
	}
	const char *GetModelName(const model_t *mod)
	{
		using Fn = char*(__thiscall*)(IVModelInfo*, const model_t *);
		return (*(Fn**)this)[3](this, mod);
	}
	studiohdr_t	*GetStudiomodel(const model_t *mod)
	{
		using Fn = studiohdr_t*(__thiscall*)(IVModelInfo*, const model_t *);
		return (*(Fn**)this)[32](this, mod);
	}
};

extern IVModelInfo* csgo_model_info;