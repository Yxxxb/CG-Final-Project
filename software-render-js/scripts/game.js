import { Player } from "./player.js";
import { Bitmap } from "./bitmap.js";
import { View } from "./view.js";
import * as Util from "./utils.js";
import * as Resources from "./resources.js";
import { Constants } from "./constants.js";

export class Game
{
    constructor()
    {
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

        // View class
        this.view;

        this.keys = {};
        this.mouse = { down: false, lastX: 0.0, lastY: 0.0, currX: 0.0, currY: 0.0, dx: 0.0, dy: 0.0 };

        this.postprocessEnabled = [false, false, true, false, false];
    }

    start()
    {
        this.init();
        this.run();
    }

    init()
    {
        this.cvs = document.getElementById("canvas");
        this.gfx = this.cvs.getContext("2d");

        this.gfx.font = "60px verdana";
        this.gfx.fillText("Loading...", 10, 60);

        this.tmpCvs = document.createElement("canvas");
        this.tmpGfx = this.tmpCvs.getContext("2d");

        // 绑定index内右侧调节按钮
        this.resBtns.push(document.getElementById("res1"));
        this.resBtns.push(document.getElementById("res2"));
        this.resBtns.push(document.getElementById("res4"));
        this.resBtns.push(document.getElementById("res5"));
        this.resBtns.push(document.getElementById("res8"));
        this.resBtns.push(document.getElementById("res10"));
        this.resBtns.push(document.getElementById("res20"));

        // 右侧像素按钮调节函数
        function reloadView(index)
        {
            console.log(index)
            if (index == Constants.SCALES[index]) return;

            const newWidth = Constants.WIDTH * Constants.SCALE / Constants.SCALES[index];
            const newHeight = Constants.HEIGHT * Constants.SCALE / Constants.SCALES[index];
            this.view = new View(newWidth, newHeight, this.player);

            Constants.WIDTH = newWidth;
            Constants.HEIGHT = newHeight;
            Constants.SCALE = Constants.SCALES[index];
            Constants.FOV = Constants.HEIGHT;

            this.tmpCvs.width = Constants.WIDTH;
            this.tmpCvs.height = Constants.HEIGHT;

            for (const btn of this.resBtns) btn.style.backgroundColor = "white";
            this.resBtns[index].style.backgroundColor = "black";
        }

        // 绑定点击事件
        for (let i = 0; i < this.resBtns.length; i++)
        {
            const btn = this.resBtns[i];
            btn.onclick = () => reloadView.bind(this)(i);
        }

        this.resBtns[Constants.SCALE_INDEX].style.backgroundColor = "black";


        // 右侧背景调节
        this.pspBtns.push(document.getElementById("psp1"));
        this.pspBtns.push(document.getElementById("psp2"));
        this.pspBtns.push(document.getElementById("psp3"));
        this.pspBtns.push(document.getElementById("psp4"));
        this.pspBtns.push(document.getElementById("psp5"));

        function setPostProcess(index)
        {
            this.postprocessEnabled[index] = !this.postprocessEnabled[index];
            this.pspBtns[index].style.backgroundColor = this.postprocessEnabled[index] ? "black" : "white";
        }

        // 绑定点击事件
        for (let i = 0; i < this.pspBtns.length; i++)
        {
            const btn = this.pspBtns[i];
            btn.onclick = () => setPostProcess.bind(this)(i);
            if (this.postprocessEnabled[i]) btn.style.backgroundColor = "black";
        }


        // 读resourse内的textures
        for (const key in Resources.textures)
        {
            if (Object.hasOwnProperty.call(Resources.textures, key))
            {
                const imageURL = Resources.textures[key][0];
                const imageWidth = Resources.textures[key][1][0];
                const imageHeight = Resources.textures[key][1][1];

                let image = new Image();
                image.src = imageURL;
                image.crossOrigin = "Anonymous";
                image.onload = () =>
                {
                    this.tmpCvs.setAttribute("width", imageWidth + "px");
                    this.tmpCvs.setAttribute("height", imageHeight + "px");
                    // Loading textures.

                    // 将图片贴上去 不加此步骤的话只有黑色的模型
                    this.tmpGfx.drawImage(image, 0, 0, imageWidth, imageHeight);

                    // 对天空盒进行处理
                    if (key == "skybox")
                    {
                        // 取边长 整数
                        const size = Util.int(imageWidth / 4);

                        // 分割天空盒图片
                        let top = this.tmpGfx.getImageData(size, 0, size, size);
                        let bottom = this.tmpGfx.getImageData(size, size * 2, size, size);
                        let front = this.tmpGfx.getImageData(size, size, size, size);
                        let back = this.tmpGfx.getImageData(size * 3, size, size, size);
                        let right = this.tmpGfx.getImageData(size * 2, size, size, size);
                        let left = this.tmpGfx.getImageData(0, size, size, size);

                        // 图片转rgb 存储在resources里面
                        Resources.textures["skybox_top"] = Util.convertImageDataToBitmap(top, size, size);
                        Resources.textures["skybox_bottom"] = Util.convertImageDataToBitmap(bottom, size, size);
                        Resources.textures["skybox_front"] = Util.convertImageDataToBitmap(front, size, size);
                        Resources.textures["skybox_back"] = Util.convertImageDataToBitmap(back, size, size);
                        Resources.textures["skybox_right"] = Util.convertImageDataToBitmap(right, size, size);
                        Resources.textures["skybox_left"] = Util.convertImageDataToBitmap(left, size, size);

                        Constants.loadedResources++;
                        return;
                    }

                    // 同样 转rgb存储
                    image = this.tmpGfx.getImageData(0, 0, imageWidth, imageHeight);
                    image = Util.convertImageDataToBitmap(image, imageWidth, imageHeight);

                    Resources.textures[key] = image;
                    Constants.loadedResources++;
                }
            }
        }


        // 绑定鼠标键盘点击事件
        window.addEventListener("mousedown", (e) =>
        {
            if (e.button != 0) return;

            this.mouse.down = true;
        }, false);
        window.addEventListener("mouseup", (e) =>
        {
            if (e.button != 0) return;

            this.mouse.down = false;
        }, false);

        window.addEventListener("keydown", (e) =>
        {
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

        window.addEventListener("keyup", (e) =>
        {
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

        window.addEventListener("mousemove", (e) =>
        {
            this.mouse.currX = e.screenX;
            this.mouse.currY = e.screenY;
        });

        // fps指示
        this.frameCounterElement = document.getElementById("frame_counter");

        Constants.WIDTH = Constants.WIDTH / Constants.SCALE;
        Constants.HEIGHT = Constants.HEIGHT / Constants.SCALE;

        // 创建player和View对象
        // player内可修改水平移动速度、视角移动速度等等
        this.player = new Player(this.keys, this.mouse);
        this.view = new View(Constants.WIDTH, Constants.HEIGHT, this.player);

        let sample = new Bitmap(64, 64);
        for (let i = 0; i < 64 * 64; i++)
        {
            const x = i % 64;
            const y = Util.int(i / 64);
            sample.pixels[i] = (((x << 6) % 0xff) << 8) | (y << 6) % 0xff;
        }
        Resources.textures["sample0"] = sample;

        sample = new Bitmap(64, 64);
        sample.clear(0xff00ff);
        Resources.textures["sample1"] = sample;

        sample = new Bitmap(64, 64);
        sample.clear(0xffffff);
        Resources.textures["white"] = sample;

        sample = new Bitmap(64, 64);
        sample.clear(0x8080ff);
        Resources.textures["default_normal"] = sample;
    }

    run()
    {
        // 精确到ms的时间戳
        const now = performance.now();
        // 清空times
        while (this.times.length > 0 && this.times[0] <= now - 1000) this.times.shift();

        const delta = (now - this.times[this.times.length - 1]) / 1000.0;

        this.times.push(now);
        this.fps = this.times.length;
        this.frameCounterElement.innerHTML = this.fps + "fps";

        if (!this.started && Constants.loadedResources == Constants.resourceReady)
        {
            this.started = true;
            // canvas大小
            this.cvs.setAttribute("width", Constants.WIDTH * Constants.SCALE + "px");
            this.cvs.setAttribute("height", Constants.HEIGHT * Constants.SCALE + "px");
            this.tmpCvs.setAttribute("width", Constants.WIDTH * Constants.SCALE + "px");
            this.tmpCvs.setAttribute("height", Constants.HEIGHT * Constants.SCALE + "px");
            this.gfx.font = "48px verdana";
        }
        if (!this.started)
        {
            this.gfx.clearRect(0, 0, this.cvs.width, this.cvs.height);
            // 加载模型的进度 上文每次load一个obj或texture都会loadedResources++
            this.gfx.fillText("Loading..." + Util.int(Constants.loadedResources / Constants.resourceReady * 100) + "%", 10, 60);
        }

        if (this.started && !this.pause)
        {
            // update view & player
            this.update(delta);
            // view
            this.render();
            this.time += delta;
        }
        else if (this.pause)
        {
            this.gfx.fillText("PAUSE", 4, 40);
        }

        // 必须加 每刷新界面的时候重新执行this
        requestAnimationFrame(this.run.bind(this));
    }

    update(delta)
    {
        this.mouse.dx = this.mouse.currX - this.mouse.lastX;
        this.mouse.dy = this.mouse.currY - this.mouse.lastY;
        this.mouse.lastX = this.mouse.currX;
        this.mouse.lastY = this.mouse.currY;

        this.player.update(delta);
        this.view.update(delta);
    }

    render()
    {
        this.view.clear(0x808080);
        // 此步骤开始模型与贴图的放置与渲染
        this.view.renderView();
        this.view.postProcess(this.postprocessEnabled);

        if (true)
        {
            this.tmpGfx.putImageData(Util.convertBitmapToImageData(this.view), 0, 0);
            this.gfx.save();
            this.gfx.imageSmoothingEnabled = false;
            this.gfx.scale(Constants.SCALE, Constants.SCALE);
            this.gfx.drawImage(this.tmpCvs, 0, 0);
            this.gfx.restore();
        } else
        {
            this.gfx.putImageData(Util.convertBitmapToImageData(this.view, Constants.SCALE), 0, 0)
        }
    }
}