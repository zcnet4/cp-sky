#  cp1.gypģ������
# cpcheck.gypi
{
  'targets': [
    {
	  # Ŀ�깤����
      'target_name': 'cpcheck',
      'type': '<(component)',
	  # Ŀ�깤����������
      'dependencies': [
		'../base/base.gyp:base',
		'../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
      ],
	  #Ŀ�깤��Ԥ����궨��
      'defines': [
		'CPCHECK_IMPLEMENTATION',
      ],
	  #Ŀ�깤��C++ includeĿ¼
      'include_dirs': [
        '.',
		'..',
      ],
	  #Ŀ�깤��Դ����·��
      'sources': [
		'cpcheck/cpcheck_export.h',
		'cpcheck/cpcheck_api.h',
		'cpcheck/cpcheck_api.cc',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '..',
        ],
      },
      'conditions': [
        ['OS=="linux"', {
          'defines': [
            'LINUX_DEFINE',
          ],
          'include_dirs': [
            'include/linux',
          ],
        }],
        ['OS=="win"', {
          'defines': [
            'WINDOWS_SPECIFIC_DEFINE',
          ],
        }, { # OS != "win",
          'defines': [
            'NON_WINDOWS_DEFINE',
          ],
        }]
      ],
    },
  ],
}
