// PX2FBXMat.hpp

#ifndef PX2FBXMAT_HPP
#define PX2FBXMAT_HPP

#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2Float3.hpp"

namespace PX2
{

	class Mtl
	{
	public:
		std::string mName;
		Float3 mAmbient;
		Float3 mDiffuse;
		Float3 mEmissive;
		float mTransparencyFactor;
		std::string mDiffuseMapName;
		std::string mEmissiveMapName;
		std::string mGlossMapName;
		std::string mNormalMapName;
		std::string mSpecularMapName;

		int mNumTriangles;
	};

	class LambertMtl : public Mtl
	{
	public:
	};

	class PhongMtl : public Mtl
	{
	public:
		Float3 mSpecular;
		Float3 mReflection;
		double mSpecularPower;
		double mShininess;
		double mReflectionFactor;
	};

}

#endif