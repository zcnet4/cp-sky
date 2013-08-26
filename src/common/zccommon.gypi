#  zccommon.gypģ������
# zccommon.gypi
{
  'targets': [
    {
	  # Ŀ�깤����
      'target_name': 'zccommon',
      'type': '<(component)',
	  # Ŀ�깤����������
      'dependencies': [
		'../base/base.gyp:base',
		'../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
      ],
	  #Ŀ�깤��Ԥ����궨��
      'defines': [
		'ZCCOMMON_IMPLEMENTATION',
      ],
	  #Ŀ�깤��C++ includeĿ¼
      'include_dirs': [
        '.',
		'..',
      ],
	  #Ŀ�깤��Դ����·��
      'sources': [
			'zccommon_export.h',
			'zccommon.h',
			'xml/hm_xml.h',
			'xml/hm_xml.cpp',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '..',
        ],
      },
    },
  ],
}
