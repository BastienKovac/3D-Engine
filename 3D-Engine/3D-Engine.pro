QT       += core gui opengl charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -L/usr/local/lib

# Include openGL lib
win32 {
    LIBS += -lopengl32
}

linux-g++* {
    LIBS += -lGLU
}


QMAKE_CXXFLAGS += -std=c++17
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3D/Camera/camera.cpp \
    3D/openglbase.cpp \
    Spline/bspline.cpp \
    UI/bsplinechart.cpp \
    glm/detail/glm.cpp \
    main.cpp \
    UI/mainwindow.cpp \
    3D/openglrenderer.cpp \
    3D/Geometry/geometry.cpp \
    3D/Display/simplescene.cpp

HEADERS += \
    3D/Camera/camera.h \
    3D/openglbase.h \
    Spline/bspline.h \
    UI/bsplinechart.h \
    UI/mainwindow.h \
    3D/openglrenderer.h \
    glm/common.hpp \
    glm/detail/_features.hpp \
    glm/detail/_fixes.hpp \
    glm/detail/_noise.hpp \
    glm/detail/_swizzle.hpp \
    glm/detail/_swizzle_func.hpp \
    glm/detail/_vectorize.hpp \
    glm/detail/func_common.hpp \
    glm/detail/func_common.inl \
    glm/detail/func_common_simd.inl \
    glm/detail/func_exponential.hpp \
    glm/detail/func_exponential.inl \
    glm/detail/func_exponential_simd.inl \
    glm/detail/func_geometric.hpp \
    glm/detail/func_geometric.inl \
    glm/detail/func_geometric_simd.inl \
    glm/detail/func_integer.hpp \
    glm/detail/func_integer.inl \
    glm/detail/func_integer_simd.inl \
    glm/detail/func_matrix.hpp \
    glm/detail/func_matrix.inl \
    glm/detail/func_matrix_simd.inl \
    glm/detail/func_packing.hpp \
    glm/detail/func_packing.inl \
    glm/detail/func_packing_simd.inl \
    glm/detail/func_trigonometric.hpp \
    glm/detail/func_trigonometric.inl \
    glm/detail/func_trigonometric_simd.inl \
    glm/detail/func_vector_relational.hpp \
    glm/detail/func_vector_relational.inl \
    glm/detail/func_vector_relational_simd.inl \
    glm/detail/precision.hpp \
    glm/detail/setup.hpp \
    glm/detail/type_float.hpp \
    glm/detail/type_gentype.hpp \
    glm/detail/type_gentype.inl \
    glm/detail/type_half.hpp \
    glm/detail/type_half.inl \
    glm/detail/type_int.hpp \
    glm/detail/type_mat.hpp \
    glm/detail/type_mat.inl \
    glm/detail/type_mat2x2.hpp \
    glm/detail/type_mat2x2.inl \
    glm/detail/type_mat2x3.hpp \
    glm/detail/type_mat2x3.inl \
    glm/detail/type_mat2x4.hpp \
    glm/detail/type_mat2x4.inl \
    glm/detail/type_mat3x2.hpp \
    glm/detail/type_mat3x2.inl \
    glm/detail/type_mat3x3.hpp \
    glm/detail/type_mat3x3.inl \
    glm/detail/type_mat3x4.hpp \
    glm/detail/type_mat3x4.inl \
    glm/detail/type_mat4x2.hpp \
    glm/detail/type_mat4x2.inl \
    glm/detail/type_mat4x3.hpp \
    glm/detail/type_mat4x3.inl \
    glm/detail/type_mat4x4.hpp \
    glm/detail/type_mat4x4.inl \
    glm/detail/type_mat4x4_simd.inl \
    glm/detail/type_vec.hpp \
    glm/detail/type_vec.inl \
    glm/detail/type_vec1.hpp \
    glm/detail/type_vec1.inl \
    glm/detail/type_vec2.hpp \
    glm/detail/type_vec2.inl \
    glm/detail/type_vec3.hpp \
    glm/detail/type_vec3.inl \
    glm/detail/type_vec4.hpp \
    glm/detail/type_vec4.inl \
    glm/detail/type_vec4_simd.inl \
    glm/exponential.hpp \
    glm/ext.hpp \
    glm/fwd.hpp \
    glm/geometric.hpp \
    glm/glm.hpp \
    glm/gtc/bitfield.hpp \
    glm/gtc/bitfield.inl \
    glm/gtc/color_space.hpp \
    glm/gtc/color_space.inl \
    glm/gtc/constants.hpp \
    glm/gtc/constants.inl \
    glm/gtc/epsilon.hpp \
    glm/gtc/epsilon.inl \
    glm/gtc/functions.hpp \
    glm/gtc/functions.inl \
    glm/gtc/integer.hpp \
    glm/gtc/integer.inl \
    glm/gtc/matrix_access.hpp \
    glm/gtc/matrix_access.inl \
    glm/gtc/matrix_integer.hpp \
    glm/gtc/matrix_inverse.hpp \
    glm/gtc/matrix_inverse.inl \
    glm/gtc/matrix_transform.hpp \
    glm/gtc/matrix_transform.inl \
    glm/gtc/noise.hpp \
    glm/gtc/noise.inl \
    glm/gtc/packing.hpp \
    glm/gtc/packing.inl \
    glm/gtc/quaternion.hpp \
    glm/gtc/quaternion.inl \
    glm/gtc/quaternion_simd.inl \
    glm/gtc/random.hpp \
    glm/gtc/random.inl \
    glm/gtc/reciprocal.hpp \
    glm/gtc/reciprocal.inl \
    glm/gtc/round.hpp \
    glm/gtc/round.inl \
    glm/gtc/type_aligned.hpp \
    glm/gtc/type_precision.hpp \
    glm/gtc/type_precision.inl \
    glm/gtc/type_ptr.hpp \
    glm/gtc/type_ptr.inl \
    glm/gtc/ulp.hpp \
    glm/gtc/ulp.inl \
    glm/gtc/vec1.hpp \
    glm/gtc/vec1.inl \
    glm/gtx/associated_min_max.hpp \
    glm/gtx/associated_min_max.inl \
    glm/gtx/bit.hpp \
    glm/gtx/bit.inl \
    glm/gtx/closest_point.hpp \
    glm/gtx/closest_point.inl \
    glm/gtx/color_encoding.hpp \
    glm/gtx/color_encoding.inl \
    glm/gtx/color_space.hpp \
    glm/gtx/color_space.inl \
    glm/gtx/color_space_YCoCg.hpp \
    glm/gtx/color_space_YCoCg.inl \
    glm/gtx/common.hpp \
    glm/gtx/common.inl \
    glm/gtx/compatibility.hpp \
    glm/gtx/compatibility.inl \
    glm/gtx/component_wise.hpp \
    glm/gtx/component_wise.inl \
    glm/gtx/dual_quaternion.hpp \
    glm/gtx/dual_quaternion.inl \
    glm/gtx/euler_angles.hpp \
    glm/gtx/euler_angles.inl \
    glm/gtx/extend.hpp \
    glm/gtx/extend.inl \
    glm/gtx/extended_min_max.hpp \
    glm/gtx/extended_min_max.inl \
    glm/gtx/fast_exponential.hpp \
    glm/gtx/fast_exponential.inl \
    glm/gtx/fast_square_root.hpp \
    glm/gtx/fast_square_root.inl \
    glm/gtx/fast_trigonometry.hpp \
    glm/gtx/fast_trigonometry.inl \
    glm/gtx/float_notmalize.inl \
    glm/gtx/gradient_paint.hpp \
    glm/gtx/gradient_paint.inl \
    glm/gtx/handed_coordinate_space.hpp \
    glm/gtx/handed_coordinate_space.inl \
    glm/gtx/hash.hpp \
    glm/gtx/hash.inl \
    glm/gtx/integer.hpp \
    glm/gtx/integer.inl \
    glm/gtx/intersect.hpp \
    glm/gtx/intersect.inl \
    glm/gtx/io.hpp \
    glm/gtx/io.inl \
    glm/gtx/log_base.hpp \
    glm/gtx/log_base.inl \
    glm/gtx/matrix_cross_product.hpp \
    glm/gtx/matrix_cross_product.inl \
    glm/gtx/matrix_decompose.hpp \
    glm/gtx/matrix_decompose.inl \
    glm/gtx/matrix_interpolation.hpp \
    glm/gtx/matrix_interpolation.inl \
    glm/gtx/matrix_major_storage.hpp \
    glm/gtx/matrix_major_storage.inl \
    glm/gtx/matrix_operation.hpp \
    glm/gtx/matrix_operation.inl \
    glm/gtx/matrix_query.hpp \
    glm/gtx/matrix_query.inl \
    glm/gtx/matrix_transform_2d.hpp \
    glm/gtx/matrix_transform_2d.inl \
    glm/gtx/mixed_product.hpp \
    glm/gtx/mixed_product.inl \
    glm/gtx/norm.hpp \
    glm/gtx/norm.inl \
    glm/gtx/normal.hpp \
    glm/gtx/normal.inl \
    glm/gtx/normalize_dot.hpp \
    glm/gtx/normalize_dot.inl \
    glm/gtx/number_precision.hpp \
    glm/gtx/number_precision.inl \
    glm/gtx/optimum_pow.hpp \
    glm/gtx/optimum_pow.inl \
    glm/gtx/orthonormalize.hpp \
    glm/gtx/orthonormalize.inl \
    glm/gtx/perpendicular.hpp \
    glm/gtx/perpendicular.inl \
    glm/gtx/polar_coordinates.hpp \
    glm/gtx/polar_coordinates.inl \
    glm/gtx/projection.hpp \
    glm/gtx/projection.inl \
    glm/gtx/quaternion.hpp \
    glm/gtx/quaternion.inl \
    glm/gtx/range.hpp \
    glm/gtx/raw_data.hpp \
    glm/gtx/raw_data.inl \
    glm/gtx/rotate_normalized_axis.hpp \
    glm/gtx/rotate_normalized_axis.inl \
    glm/gtx/rotate_vector.hpp \
    glm/gtx/rotate_vector.inl \
    glm/gtx/scalar_multiplication.hpp \
    glm/gtx/scalar_relational.hpp \
    glm/gtx/scalar_relational.inl \
    glm/gtx/spline.hpp \
    glm/gtx/spline.inl \
    glm/gtx/std_based_type.hpp \
    glm/gtx/std_based_type.inl \
    glm/gtx/string_cast.hpp \
    glm/gtx/string_cast.inl \
    glm/gtx/transform.hpp \
    glm/gtx/transform.inl \
    glm/gtx/transform2.hpp \
    glm/gtx/transform2.inl \
    glm/gtx/type_aligned.hpp \
    glm/gtx/type_aligned.inl \
    glm/gtx/type_trait.hpp \
    glm/gtx/type_trait.inl \
    glm/gtx/vec_swizzle.hpp \
    glm/gtx/vector_angle.hpp \
    glm/gtx/vector_angle.inl \
    glm/gtx/vector_query.hpp \
    glm/gtx/vector_query.inl \
    glm/gtx/wrap.hpp \
    glm/gtx/wrap.inl \
    glm/integer.hpp \
    glm/mat2x2.hpp \
    glm/mat2x3.hpp \
    glm/mat2x4.hpp \
    glm/mat3x2.hpp \
    glm/mat3x3.hpp \
    glm/mat3x4.hpp \
    glm/mat4x2.hpp \
    glm/mat4x3.hpp \
    glm/mat4x4.hpp \
    glm/matrix.hpp \
    glm/packing.hpp \
    glm/simd/common.h \
    glm/simd/exponential.h \
    glm/simd/geometric.h \
    glm/simd/integer.h \
    glm/simd/matrix.h \
    glm/simd/packing.h \
    glm/simd/platform.h \
    glm/simd/trigonometric.h \
    glm/simd/vector_relational.h \
    glm/trigonometric.hpp \
    glm/vec2.hpp \
    glm/vec3.hpp \
    glm/vec4.hpp \
    glm/vector_relational.hpp \
    utils.h \
    3D/Geometry/geometry.h \
    3D/Display/simplescene.h

FORMS += \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshCore

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/libOpenMeshCore.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/OpenMesh/lib/release/ -lOpenMeshTools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/OpenMesh/lib/debug/ -lOpenMeshTools
else:unix: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshTools

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/release/libOpenMeshTools.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/debug/libOpenMeshTools.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/release/OpenMeshTools.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/debug/OpenMeshTools.lib
else:unix: PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/libOpenMeshTools.a

unix:!macx: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshCore

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/libOpenMeshCore.a

unix:!macx: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshCore

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include

unix:!macx: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshTools

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/OpenMesh/lib/libOpenMeshTools.a

unix:!macx: LIBS += -L$$PWD/libs/OpenMesh/lib/ -lOpenMeshTools

INCLUDEPATH += $$PWD/libs/OpenMesh/include
DEPENDPATH += $$PWD/libs/OpenMesh/include
