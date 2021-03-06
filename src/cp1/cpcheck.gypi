#  cp1.gyp模板配置
# cpcheck.gypi
{
  'targets': [
    {
	  # 目标工程名
      'target_name': 'cpcheck',
      'type': '<(component)',
	  # 目标工程依赖工程
      'dependencies': [
		'../base/base.gyp:base',
		'../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
		'../common/zccommon.gyp:zccommon',
      ],
	  #目标工程预处理宏定义
      'defines': [
		'CPCHECK_IMPLEMENTATION',
      ],
	  #目标工程C++ include目录
      'include_dirs': [
        '.',
		'..',
      ],
	  #目标工程源代码路径
      'sources': [
		'cpcheck/cpcheck_export.h',
		#'cpcheck/cpcheck_api.h',
		#'cpcheck/cpcheck_api.cc',
		'cpcheck/lib_data/lib_data.h',
		'cpcheck/lib_data/lib_data.cc',
		'cpcheck/cp_av_check_impl.h',
		'cpcheck/cp_av_check_impl.cc',
		'cpcheck/read_xml_file.h',
		'cpcheck/read_xml_file.cc',
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
	{
	  # 目标工程名
      'target_name': 'cpcheck_test',
      'type': 'executable',
	  # 目标工程依赖工程
      'dependencies': [
		'../base/base.gyp:base',
		'../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
		'../common/zccommon.gyp:zccommon',
		'../cp1/cp1.gyp:cpcheck',
      ],
	  #目标工程源代码路径
      'sources': [
		'cp_main.cc',
      ],
    },
  ],
}
