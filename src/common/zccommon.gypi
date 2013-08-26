#  zccommon.gyp模板配置
# zccommon.gypi
{
  'targets': [
    {
	  # 目标工程名
      'target_name': 'zccommon',
      'type': '<(component)',
	  # 目标工程依赖工程
      'dependencies': [
		'../base/base.gyp:base',
		'../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
      ],
	  #目标工程预处理宏定义
      'defines': [
		'ZCCOMMON_IMPLEMENTATION',
      ],
	  #目标工程C++ include目录
      'include_dirs': [
        '.',
		'..',
      ],
	  #目标工程源代码路径
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
