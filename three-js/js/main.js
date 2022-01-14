function Main()
{
    var scope=this;
    this.VR=false;
    this.scene=new THREE.Scene();
    var width=window.innerWidth;
    var height=window.innerHeight;
    this.renderer;
    this.camera=new THREE.PerspectiveCamera(70,width/height,0.1,1000);
    this.start=function(){
        this.init();
        this.animate();
    }
    this.init=function()
    {
        this.renderer=new THREE.WebGLRenderer();
        this.renderer.autoClear=true;
        // this.renderer.setPixelRatio(window.devicePixelRatio);

        // this.renderer.gammaInput=true;
        // this.renderer.gammaOutput=true;
        var axes = new THREE.AxisHelper(500);
        this.scene.add(axes);
        this.renderer.setSize(width,height);
        this.renderer.setClearColor(0xffffff,1.0);
        document.body.appendChild(this.renderer.domElement);

        this.camera.position.set(10,10,50);
        this.camera.lookAt(new THREE.Vector3(10,0,0));

        this.ambient=new THREE.AmbientLight(0xffffff,1);
        this.scene.add(this.ambient);
    }
    this.animate=function (){
        scope.renderer.render(scope.scene,scope.camera);
        requestAnimationFrame(scope.animate);
    }
}