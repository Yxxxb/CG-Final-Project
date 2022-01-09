### 高级OpenGL

#### 深度测试

使用深度缓冲来防止3D为物体面的前后关系错乱。深度缓冲会存储每个片段的信息，以16、24或32位float的形式储存它的深度值。深度测试被启用时，OpenGL会将一个片段的深度值与深度缓冲中的内容进行比较，取较大值，并对深度缓冲进行更新，较小值（片段）被丢弃。具体体现为：每个片段的深度值都位于[0,1]范围之中（或者被线性变换到[0,1]之间），系统通过比较深度缓冲中深度值的大小和物体z值大小来判断片段的远近，并更新深度缓冲。

- ```c++
  glEnable(GL_DEPTH_TEST)
  ```

  启用深度测试，启用后会对每一个片段的深度进行比较，并取较大值进行更新，较小值丢弃。

- ```C++
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
  ```

  清空深度缓冲。在每一轮渲染之前应该清空深度缓冲进行更新。

- ```C++
  glDepthFunc(GL_LESS);
  ```

  深度测试函数，用于是我们能够控制何时通过或丢弃片段，何时取更新深度缓冲。

- 深度冲突：当物体靠的过近时，由于深度测试，会表现出视野中紧贴的两个面不断切换的情况。应该避免两个物体的面重合。

#### 混合

混合用于实现物体的透明度，例如透过一块红色的玻璃去观察后面的物体，此时物体的颜色就为红色与它原本颜色的混合色。

- alpha值：Alpha颜色值是颜色向量的第四个分量。1.0时物体完全不透明，0.0时物体将会是完全透明的。当alpha值为0.5时，物体的颜色有50%是来自物体自身的颜色，50%来自背后物体的颜色。

- ```C++
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  ```

  此函数中在纹理加载时使用了alpha通道，用于之后设置透明度。

- ```
  void main()
  {             
      vec4 texColor = texture(texture1, TexCoords);
      if(texColor.a < 0.1)
          discard;
      FragColor = texColor;
  }
  ```

  加载物体纹理时，需要在片段着色器中对alpha通道的值进行判断。当采样的纹理颜色小于0.1时，丢弃该片段，以此实现只显示想要的部分纹理，而其余部分透明。

- ```C++
  glEnable(GL_BLEND)
  ```

  启用颜色混合

- ```
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
  ```

  颜色混合函数。第一项为第一个物体颜色的alpha通道值，第二项为另一个物体颜色的alpha通道值，二者和为1。即此函数设置了物体颜色混合时每部分的占比。

- 在使用**混合**时要注意**深度测试**（一般情况时应该从远到近依次渲染），否则深度测试会优先执行，在设置物体透明度后，还是会因为深度测试看不到后面的物体。

#### 面剔除

```
glEnable(GL_CULL_FACE);
```

启用面剔除，检查所有面向观察者的面进行渲染，丢弃那些背向的面，节省片段着色器调用开销。

- 方法：检测顶点环绕顺序。我们首先为三角形的顶点标号（1，2，3），以设置环绕顺序。当检测到观察者视角中该面中的点为顺时针环绕时，判断为正面并进行渲染；检测到为逆时针时，判断为背面并丢弃。

#### 立方体贴图

立方体贴图是一个包含了6个2D纹理的纹理，每个2D纹理都组成了立方体的一个面，是一张完整的纹理。

- ```
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  ```

  定义一个表示纹理ID的变量，并生成一个纹理，通过**glBindTexture**将纹理绑定到目标上

- 天空盒

  天空盒贴图与一般的立方体贴图相同，但需要注意天空盒包裹在最外层，所以应该移除该立方体的全部位移。

  ```
  glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()))
  ```

  我们通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分，我们可以将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵。通过以上代码，我们只保留了天空盒的旋转变换，实现了天空盒效果。

- 环境映射

  - 反射

    根据观察方向向量***I***和物体的法向量***N***，来计算反射向量***R***。我们使用GLSL内建的**reflect**函数来计算这个反射向量。我们以***R***向量作为索引向量来对天空盒贴图进行采样，返回环境的颜色值。最终的结果是物体看起来反射了天空盒。片段着色器部分入下：

    ```
    void main()
    {             
        vec3 I = normalize(Position - cameraPos);
        vec3 R = reflect(I, normalize(Normal));
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    ```

  - 折射

    根据观察方向向量***I***和物体的法向量***N***，来计算反射向量***R‘***，我们使用GLSL内建的**refract**函数来计算这个折射向量。后续取样操作与反射贴图相同。

    ```
    void main()
    {             
        float ratio = 1.00 / 1.52;
        vec3 I = normalize(Position - cameraPos);
        vec3 R = refract(I, normalize(Normal), ratio);
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    ```

