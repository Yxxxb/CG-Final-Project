let textures =
{
    "pepe": ["../image&obj repo/texture/pepe.png", [512, 512]],
    "dulri": ["../image&obj repo/texture/blue.png", [967, 966]],
    "container": ["../image&obj repo/texture/container2.png", [500, 500]],
    // "skybox": ["../image&obj repo/skybox/world0/day_sky.png", [3072, 2048]],
    "sp3_ft": ["../image&obj repo/skybox/textures/skybox3_pz.jpg", [1024, 1024]],
    "sp3_bk": ["../image&obj repo/skybox/textures/skybox3_nz.jpg", [1024, 1024]],
    "sp3_lf": ["../image&obj repo/skybox/textures/skybox3_nx.jpg", [1024, 1024]],
    "sp3_rt": ["../image&obj repo/skybox/textures/skybox3_px.jpg", [1024, 1024]],
    "sp3_up": ["../image&obj repo/skybox/textures/skybox3_py.jpg", [1024, 1024]],
    "sp3_dn": ["../image&obj repo/skybox/textures/skybox3_ny.jpg", [1024, 1024]],
    // "skybox": ["https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/imgs/skybox.png", [1024, 768]],
    // "skybox": ["https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/imgs/skybox2.png", [1024, 768]],
    // "skybox": ["https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/imgs/skybox3.png", [2048, 1536]],
    "brickwall": ["../image&obj repo/texture/brickwall.png", [1024, 1024]],
    "brickwall_normal": ["../image&obj repo/texture/brickwall_normal.png", [1024, 1024]],
    "brick": ["../image&obj repo/texture/bricks3.png", [1024, 1024]],
    "brick_normal": ["../image&obj repo/texture/bricks3_normal.png", [1024, 1024]],
    "stone1": ["../image&obj repo/texture/stone1_diffuse.png", [1024, 1024]],
    "stone1_normal": ["../image&obj repo/texture/stone1_normal.png", [1024, 1024]],
    "stone2": ["../image&obj repo/texture/stone2_diffuse.jpg", [1024, 1024]],
    "stone2_normal": ["../image&obj repo/texture/stone2_normal.jpg", [1024, 1024]],
    "man": ["../image&obj repo/texture/Ninja_T.png", [2048, 2048]],
};

let models =
{
    // "cube": "https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/models/cube.obj",
    // "sphere": "https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/models/sphere.obj",
    // "sphere2": "https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/models/sphere2.obj",
    // "monkey": "https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/models/monkey2.obj",
    // "man": "https://raw.githubusercontent.com/Sopiro/js_bitmap_renderer/master/models/man.obj",
    "cube": "../image&obj repo/obj/cube.obj",
    "sphere": "../image&obj repo/obj/sphere.obj",
    "sphere2": "../image&obj repo/obj/sphere.obj",
    "monkey": "../image&obj repo/obj/monkey2.obj",
    "man": "../image&obj repo/obj/ninja.obj", // 直接输入模型地址就可以导入 默认为根目录路径
};

export { models, textures};