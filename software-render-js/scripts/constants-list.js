import * as Resources from "./image-obj-resources.js";

export let ConstantsList = {}
ConstantsList.WIDTH = 1000;
ConstantsList.HEIGHT = ConstantsList.WIDTH / 4 * 3;
ConstantsList.SCALE_INDEX = 1;
// 侧边栏像素大小调整的就是这里
ConstantsList.SCALES = [1, 2, 4, 5, 8, 10, 20];
ConstantsList.SCALE = ConstantsList.SCALES[ConstantsList.SCALE_INDEX];
// 整个高度上像素个数
ConstantsList.FOV = ConstantsList.HEIGHT / ConstantsList.SCALE;
ConstantsList.resourceReady = Object.keys(Resources.textures).length + Object.keys(Resources.models).length;
ConstantsList.loadedResources = 0;
ConstantsList.globalAlpha = 255;
