include 'premake'

make_solution 'cg3lz'

-------------------------------
configuration 'windows'
	defines {
		'NOGDI',
		-- 'CROW_MSVC_WORKAROUND'
	}
configuration '*'


-------------------------------
zeromq_root = {
	x32 = [[C:\Program Files (x86)\ZeroMQ 4.0.4\]],
	x64 = [[C:\Program Files\ZeroMQ 4.0.4\]],
	osx = [[/usr/local/Cellar/zeromq/4.1.4/]] -- brew install zeromq
}

zeromq_lib = {
	win = 'libzmq-v120-mt-4_0_4',
	osx = 'zmq',
	lin = { 'zmq', 'pthread' }
}

includedirs {
	'deps/cppzmq',
}

-- libzmq config
configuration {'x32','windows'}
	includedirs { path.join(zeromq_root.x32,'include') }
	libdirs { path.join(zeromq_root.x32,'lib') }
configuration {'x64','windows'}
	includedirs { path.join(zeromq_root.x64,'include') }
	libdirs { path.join(zeromq_root.x64,'lib') }
configuration 'macosx'
	includedirs { path.join(zeromq_root.osx,'include') }
	libdirs { path.join(zeromq_root.osx,'lib') }
configuration '*'

function deploy_libzmq()
	configuration {'x32','windows'}
		postbuildcommands {
			[[xcopy "]]..path.getabsolute(path.join(zeromq_root.x32,"bin/*.dll")):gsub('/','\\')..[[" "$(TargetDir)" /s /d /y]]
		}
	configuration {'x64','windows'}
		postbuildcommands {
			[[xcopy "]]..path.getabsolute(path.join(zeromq_root.x64,"bin/*.dll")):gsub('/','\\')..[[" "$(TargetDir)" /s /d /y]]
		}
	configuration '*'
end

function link_zeromq()
	configuration 'windows'
		links {
			-- 'dbghelp',
			zeromq_lib.win
		}
	configuration 'macosx'
		links {
			zeromq_lib.osx,
		}
	configuration 'linux'
		links {
			zeromq_lib.lin,
		}
	configuration '*'
end

-------------------------------

boost = assert(dofile 'premake/recipes/boost.lua')
boost:set_defines()
boost:set_includedirs()
boost:set_libdirs()

includedirs {
	'deps/crow/include',
	'deps/crow/amalgamate',
	'deps/mstch/include',
	'deps/mstch/src',
	'deps/spdlog/include',
	'deps/picojson',
	'deps/picojson_serializer',
}

defines {
	'PICOJSON_USE_INT64'
}

--------------------------------------------------------------------
make_static_lib('mstch', {
	'deps/mstch/src/**.cpp',
	'deps/mstch/src/**.hpp'
})

use_standard('c++11')

--------------------------------------------------------------------
make_console_app('cg3lz', {
	'src/main.cpp',
	'src/spdlogger.cpp',
	'src/zeromq_log_source.cpp',
	'src/log_view.cpp',
	'src/file_contents.cpp',
	'src/resources.cpp',
	'src/main_page.cpp',
	'src/config_persistence.cpp',
	'src/*.h'
})
use_standard('c++14')

links { 'mstch' }

link_zeromq()
deploy_libzmq()

configuration 'not windows'
	links {
		'boost_system',
		'boost_date_time',
		'boost_regex',
		'boost_filesystem',
		'pthread',
		'z'
	}
configuration '*'

--------------------------------------------------------------------
make_console_app('log_some', { 'src/log_some.cpp' })
use_standard('c++14')

link_zeromq()

configuration 'not windows'
	links {
		'pthread'
	}
configuration '*'

