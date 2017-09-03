import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Extras 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import QtQuick.Scene2D 2.9
import QtMultimedia 5.9 as Mult

Entity {

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, -40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

//    OrbitCameraController {
//        camera: camera
//    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0.5, 0.5, 0.5, 1)
                camera: camera
            }
        }

//        InputSettings {}
    ]

    Scene2D {
        output: RenderTargetOutput {
                    attachmentPoint: RenderTargetOutput.Color0
                    texture: Texture2D {
                        id: textureId
                        width: 600
                        height: 600
                        format: Texture.RGBA8_UNorm
                    }
        }
        entities: [cub]

        Item {
            width: 600
            height: 600

            Mult.VideoOutput {
                source: webCamera
                anchors.fill: parent
            }
            Mult.Camera {
                id: webCamera
            }
        }
    }

//    Entity {
//        components: DirectionalLight {
//            worldDirection: Qt.vector3d(1.0, 0.0, 0.0);
//        }
//    }

    Entity {
        id: cub

        components: [ObjectPicker {},
            CuboidMesh {
                xExtent: 10
                yExtent: 10
                zExtent: 0
            },
//            PhongMaterial {
//                id: material
//                ambient: Qt.rgba(0, 0, 1, 0.8)
//                diffuse: Qt.rgba(0, 1, 1, 0.8)
//                specular: Qt.rgba(1, 1, 1, 1)
//                shininess: 1

//            },

            TextureMaterial {
//              texture: TextureLoader {
//                  source: "../AugReal/Samples/original.jpg"
//              }
              texture: textureId
            },

            Transform {
                id: transform
                property real angle: 0
                property real scaleCub: 0.5
                rotationX: angle
                rotationY: angle / 2
                scale: scaleCub
            }

        ]

        NumberAnimation {
            target: transform
            property: "angle"
            from: 0
            to: 360
            duration: 5000
            loops: Animation.Infinite
            running: true
        }

        SequentialAnimation{
            running: true
            loops: Animation.Infinite

            NumberAnimation {
                target: transform
                property: "scale"
                to: 2.5
                duration: 2000
                easing.type: Easing.InOutElastic
            }
            NumberAnimation {
                target: transform
                property: "scale"
                to: 0.5
                duration: 2000
                easing.type: Easing.InOutElastic
            }

        }
    }
}

