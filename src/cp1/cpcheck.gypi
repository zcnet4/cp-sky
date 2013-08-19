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
