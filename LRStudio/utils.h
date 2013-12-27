#pragma once

namespace LRStudio
{
	class utils
	{
	public:
		static System::String^ makeManagedString(const char *pszString)
		{
			return System::Runtime::InteropServices::Marshal::PtrToStringAnsi(System::IntPtr((void*)pszString));
		}

		static std::string makeNativeString(System::String ^str)
		{
			System::IntPtr res=System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
			std::string res_str=(const char*)res.ToPointer();
			System::Runtime::InteropServices::Marshal::FreeHGlobal(res);
			return res_str;
		}

		static LRControls::Envelope^ copyNativeEnvelope(LR::EnvelopePtr src_env)
		{
			if(!src_env)
			{
				return nullptr;
			}
			LRControls::Envelope ^dst_env=gcnew LRControls::Envelope();
			dst_env->PreBehavior=LRControls::Envelope::Behavior(src_env->getPreBehavior());
			dst_env->PostBehavior=LRControls::Envelope::Behavior(src_env->getPostBehavior());
			long key_count=src_env->getNumKeys();
			long i=0;
			for(;i<key_count;i++)
			{
				LRControls::EnvelopeKey ^key=gcnew LRControls::EnvelopeKey();
				key->Position=src_env->getKeyPositionAtIndex(i);
				key->Value=src_env->getKeyValueAtIndex(i);
				key->Tension=src_env->getKeyTensionAtIndex(i);
				key->Continuity=src_env->getKeyContinuityAtIndex(i);
				key->Bias=src_env->getKeyBiasAtIndex(i);
				key->Param1=src_env->getKeyParameter1AtIndex(i);
				key->Param2=src_env->getKeyParameter2AtIndex(i);
				key->Param3=src_env->getKeyParameter3AtIndex(i);
				key->Param4=src_env->getKeyParameter4AtIndex(i);
				key->Incoming=LRControls::EnvelopeKey::Shape(src_env->getKeyTypeAtIndex(i));
				dst_env->add_key(key);
			}
			return dst_env;
		}

		static void copyManagedEnvelope(LRControls::Envelope ^src_env,LR::EnvelopePtr dst_env)
		{
			if(!dst_env)
			{
				return;
			}
			dst_env->clear();
			if(!src_env)
			{
				return;
			}
			long key_count=src_env->KeyCount;
			long i=0;
			if(key_count==0)
			{
				return;
			}
			for(;i<key_count;i++)
			{
				long out_index=0;
				LRControls::EnvelopeKey ^key=src_env->Keys[i];
				dst_env->addKey(&out_index,key->Position,key->Value,(long)key->Incoming,
					key->Tension,key->Continuity,key->Bias,
					key->Param1,key->Param2,key->Param3,key->Param4);
			}
			dst_env->setPreBehavior((long)src_env->PreBehavior);
			dst_env->setPostBehavior((long)src_env->PostBehavior);
		}
	};

	namespace SWF=System::Windows::Forms;
	namespace ST=System::Threading;
}
