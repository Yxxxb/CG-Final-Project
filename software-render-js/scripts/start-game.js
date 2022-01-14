import {PlayerCamera} from "./player-camera.js";
import {CanvasImage} from "./canvas-image.js";
import {PlayerView} from "./player-view.js";
import * as Util from "./tool.js";
import * as Resources from "./image-obj-resources.js";
import {ConstantsList} from "./constants-list.js";

export class StartGame {
    constructor() {
        // 和frameCounterElement绑定
        this.times = [];
        this.fps;

        this.started = false;

        // 整个canvas
        this.cvs;
        this.tmpCvs;
        // Loading 字体
        this.gfx;
        this.tmpGfx;

        // 绑定像素调节按钮
        this.resBtns = [];
        // 绑定背景调节按钮
        this.pspBtns = [];

        // frame_counter 加载计时
        this.frameCounterElement;

        this.pause = false
        this.time = 0;

        // PlayerView class
        this.view;

        this.keys = {};
        this.mouse = {down: false, lastX: 0.0, lastY: 0.0, currX: 0.0, currY: 0.0, dx: 0.0, dy: 0.0};

        // 右边五个选项的初始值
        this.postprocessEnabled = [false, false, false, false, false];
    }

    start() {
        this.init();
        this.run();
    }

    init() {
        this.cvs = document.getElementById("canvas");
        this.gfx = this.cvs.getContext("2d");

        this.gfx.font = "60px verdana";
        this.gfx.fillText("Loading...", 10, 60);

        this.tmpCvs = document.createElement("canvas");
        this.tmpGfx = this.tmpCvs.getContext("2d");

        // 读resourse内的textures
        for (const key in Resources.textures) {
            if (Object.hasOwnProperty.call(Resources.textures, key)) {
                const imageURL = Resources.textures[key][0];
                const imageWidth = Resources.textures[key][1][0];
                const imageHeight = Resources.textures[key][1][1];

                let image = new Image();
                image.src = imageURL;
                image.crossOrigin = "Anonymous";
                image.onload = () => {
                    this.tmpCvs.setAttribute("width", imageWidth + "px");
                    this.tmpCvs.setAttribute("height", imageHeight + "px");
                    // Loading textures.

                    // 将图片贴上去 不加此步骤的话只有黑色的模型
                    this.tmpGfx.drawImage(image, 0, 0, imageWidth, imageHeight);

                    // 对天空盒进行处理
                    // if (key == "skybox")
                    // {
                    //     // 取边长 整数
                    //     const size = Util.int(imageWidth / 3);

                    //     // 分割天空盒图片
                    //     let top = this.tmpGfx.getImageData(size, 0, size, size);
                    //     let bottom = this.tmpGfx.getImageData(0, 0, size, size);
                    //     let front = this.tmpGfx.getImageData(size, size, size, size);
                    //     let back = this.tmpGfx.getImageData(size * 2, 0, size, size);
                    //     let right = this.tmpGfx.getImageData(size * 2, size, size, size);
                    //     let left = this.tmpGfx.getImageData(0, size, size, size);

                    //     // 图片转rgb 存储在resources里面
                    //     Resources.textures["skybox_top"] = Util.convertImageDataToBitmap(top, size, size);
                    //     Resources.textures["skybox_bottom"] = Util.convertImageDataToBitmap(bottom, size, size);
                    //     Resources.textures["skybox_front"] = Util.convertImageDataToBitmap(front, size, size);
                    //     Resources.textures["skybox_back"] = Util.convertImageDataToBitmap(back, size, size);
                    //     Resources.textures["skybox_right"] = Util.convertImageDataToBitmap(right, size, size);
                    //     Resources.textures["skybox_left"] = Util.convertImageDataToBitmap(left, size, size);

                    //     ConstantsList.loadedResources++;
                    //     return;
                    // }
                    if (key == "sp3_ft") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_front"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }
                    if (key == "sp3_bk") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_back"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }
                    if (key == "sp3_lf") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_left"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }
                    if (key == "sp3_rt") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_right"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }
                    if (key == "sp3_up") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_top"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }
                    if (key == "sp3_dn") {
                        const size = Util.int(imageWidth);
                        let pic = this.tmpGfx.getImageData(0, 0, size, size);
                        Resources.textures["skybox_bottom"] = Util.convertImageDataToBitmap(pic, size, size);
                        ConstantsList.loadedResources++;
                        return;
                    }

                    // 同样 转rgb存储
                    image = this.tmpGfx.getImageData(0, 0, imageWidth, imageHeight);
                    image = Util.convertImageDataToBitmap(image, imageWidth, imageHeight);

                    Resources.textures[key] = image;
                    ConstantsList.loadedResources++;
                }
            }
        }


        // 绑定鼠标点击事件
        window.addEventListener("mousedown", (e) => {
            if (e.button != 0) return;

            this.mouse.down = true;
            // console.log(this.mouse.down)
        }, false);
        window.addEventListener("mouseup", (e) => {
            if (e.button != 0) return;

            this.mouse.down = false;
            // console.log(this.mouse.down)

        }, false);
        //键盘某键按下后的绑定事件
        window.addEventListener("keydown", (e) => {
            if (e.key == "Escape") this.pause = !this.pause;

            if (e.key == "w" || e.key == "ArrowUp") this.keys.up = true;
            if (e.key == "a" || e.key == "ArrowLeft") this.keys.left = true;
            if (e.key == "s" || e.key == "ArrowDown") this.keys.down = true;
            if (e.key == "d" || e.key == "ArrowRight") this.keys.right = true;
            if (e.key == " ") this.keys.space = true;
            if (e.key == "c") this.keys.c = true;
            if (e.key == "q") this.keys.q = true;
            if (e.key == "e") this.keys.e = true;
            if (e.key == "Shift") this.keys.shift = true;
        });
        //键盘某键抬起后的绑定事件
        window.addEventListener("keyup", (e) => {
            if (e.key == "w" || e.key == "ArrowUp") this.keys.up = false;
            if (e.key == "a" || e.key == "ArrowLeft") this.keys.left = false;
            if (e.key == "s" || e.key == "ArrowDown") this.keys.down = false;
            if (e.key == "d" || e.key == "ArrowRight") this.keys.right = false;
            if (e.key == " ") this.keys.space = false;
            if (e.key == "c") this.keys.c = false;
            if (e.key == "q") this.keys.q = false;
            if (e.key == "e") this.keys.e = false;
            if (e.key == "Shift") this.keys.shift = false;
        });
        //鼠标移动的绑定事件
        window.addEventListener("mousemove", (e) => {
            this.mouse.currX = e.screenX;
            this.mouse.currY = e.screenY;
            // console.log(this.mouse.currX,this.mouse.currY)

        });

        // fps指示
        this.frameCounterElement = document.getElementById("frame_counter");

        ConstantsList.WIDTH = ConstantsList.WIDTH / ConstantsList.SCALE;
        ConstantsList.HEIGHT = ConstantsList.HEIGHT / ConstantsList.SCALE;

        // 创建player和View对象
        // player内可修改水平移动速度、视角移动速度等等
        //将用户与鼠标键盘绑定
        this.player = new PlayerCamera(this.keys, this.mouse);
        //将场景与用户绑定
        this.view = new PlayerView(ConstantsList.WIDTH, ConstantsList.HEIGHT, this.player);

        let sample = new CanvasImage(64, 64);
        for (let i = 0; i < 64 * 64; i++) {
            const x = i % 64;
            const y = Util.int(i / 64);
            sample.pixels[i] = (((x << 6) % 0xff) << 8) | (y << 6) % 0xff;
        }
        Resources.textures["sample0"] = sample;

        sample = new CanvasImage(64, 64);
        sample.clear(0xff00ff);
        Resources.textures["sample1"] = sample;

        sample = new CanvasImage(64, 64);
        sample.clear(0xffffff);
        Resources.textures["white"] = sample;

        sample = new CanvasImage(64, 64);
        sample.clear(0x8080ff);
        Resources.textures["default_normal"] = sample;
    }

    run() {
        // 精确到ms的时间戳
        const now = performance.now();
        // 清空times
        while (this.times.length > 0 && this.times[0] <= now - 1000) this.times.shift();
        //获取当前时间到上次变换时间的时间差并换算单位为s
        const delta = (now - this.times[this.times.length - 1]) / 1000.0;

        // console.log(now,this.times)
        this.times.push(now);
        this.fps = this.times.length;
        this.frameCounterElement.innerHTML = this.fps + "fps";
        //当游戏未开始，但是资源准备就绪时
        if (!this.started && ConstantsList.loadedResources == ConstantsList.resourceReady) {
            this.started = true;
            // canvas大小
            this.cvs.setAttribute("width", ConstantsList.WIDTH * ConstantsList.SCALE + "px");
            this.cvs.setAttribute("height", ConstantsList.HEIGHT * ConstantsList.SCALE + "px");
            this.tmpCvs.setAttribute("width", ConstantsList.WIDTH * ConstantsList.SCALE + "px");
            this.tmpCvs.setAttribute("height", ConstantsList.HEIGHT * ConstantsList.SCALE + "px");
            this.gfx.font = "48px verdana";
        }
        //当游戏未开始，但是资源在准备中时
        if (!this.started) {
            this.gfx.clearRect(0, 0, this.cvs.width, this.cvs.height);
            // 加载模型的进度 上文每次load一个obj或texture都会loadedResources++
            this.gfx.fillText("正在加载资源...", 10, 60);
        }
        //游戏开始未暂停
        if (this.started && !this.pause) {
            // update view & player
            this.update(delta);
            // view
            this.render();
            this.time += delta
            // console.log(this.time)
        }
        //游戏暂停
        else if (this.pause) {
            this.gfx.fillText("PAUSE", 4, 40);
        }

        //循环执行run函数
        // 必须加 每刷新界面的时候重新执行this
        requestAnimationFrame(this.run.bind(this));
    }

    update(delta) {
        this.mouse.dx = this.mouse.currX - this.mouse.lastX;
        this.mouse.dy = this.mouse.currY - this.mouse.lastY;
        this.mouse.lastX = this.mouse.currX;
        this.mouse.lastY = this.mouse.currY;

        this.player.update(delta);
        this.view.update(delta);
    }

    render() {
        this.view.clear(0x808080);
        // 此步骤开始模型与贴图的放置与渲染
        this.view.renderView();
        // 背景虚化等效果
        this.view.postProcess(this.postprocessEnabled);
        // 调用库函数 bitmap -> imagedata
        this.tmpGfx.putImageData(Util.convertBitmapToImageData(this.view), 0, 0);
        this.gfx.save();
        this.gfx.imageSmoothingEnabled = false;
        this.gfx.scale(ConstantsList.SCALE, ConstantsList.SCALE);
        // 调用gfx才能draw（矩阵转化成图形呈现到canvas上面）
        this.gfx.drawImage(this.tmpCvs, 0, 0);
        this.gfx.restore();
    }
}