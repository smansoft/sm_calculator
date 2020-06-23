project(cmocka-header C)

if(CMAKE_INSTALL_PROJECT_FILES)
	install(FILES
    	        cmocka.h
        	    cmocka_pbc.h
	        DESTINATION
    	        ${CMAKE_INSTALL_INCLUDEDIR}
	        COMPONENT
    	        ${PROJECT_NAME})
	if (WITH_CMOCKERY_SUPPORT)
    	install(FILES
                cmockery/cmockery.h
				cmockery/pbc.h
			DESTINATION
                ${CMAKE_INSTALL_INCLUDEDIR}/cmockery
			COMPONENT
                ${PROJECT_NAME})
	endif()
endif(CMAKE_INSTALL_PROJECT_FILES)


