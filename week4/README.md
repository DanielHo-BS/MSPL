# Nighttime Image Dehazing

## 2.1 光學模型

根據此光散射模型一張有霧霾的影像可以由下方公式所組成：

𝐈(x, y) = 𝐉(x, y)t(x, y) + 𝐀(1 − t(x, y))

- 𝐈(x, y)表示輸入的有霧霾的影像。
- A 表示大氣光(atmospheric light)。
- 𝐉(x, y)表示清晰的影像，也就是去除霧霾之影像。
- t(x, y)則表示傳輸率(transmission)，是由場景深度和大氣散射系數組成的透視圖。

在此光學模型中，輸入影像𝐈(x, y)、傳輸率t(x, y)及除霧影像𝐉(x, y)的數值都會隨座標位置(x,y)而因此改變，而大氣光 A 對於整張影像而言，我們假設其為常數。

傳輸率 t(x)是由下方方程式所組成：

t(x, y) = 𝑒^(−𝛽𝑑(𝑥,𝑦))

- β表示散射系數。
- 𝑑(𝑥, 𝑦)表示物體和相機之間的場景深度。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/f3ad4434-ef87-435f-ac4f-190380f73f09/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143623Z&X-Amz-Expires=86400&X-Amz-Signature=caa96484876812be8fa5763b32e65e620905b5d54642ab4058e16e8bf37751fe&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

## 2.2.1 除霧演算法

在 He [2]等人所提出的論文中，假設清晰無霧霾之影像 J(x,y)中是具有非常鮮豔且飽和度高的色彩，而在非常鮮豔且飽和度高的色彩空間下在顏色 RGB 的三通道中，會有一個幾乎近似於零的數值，因此提出了

組成清晰影像的暗通道先驗𝐽^(𝑑𝑎𝑟𝑘)(𝑥, 𝑦)的公式：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/32ebb870-864f-462d-826d-cbf18e3905d7/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143708Z&X-Amz-Expires=86400&X-Amz-Signature=5b67ed5ab42b01e6c62c030c9d3e87d760a115dfd5a2e411f29f94b37a215567&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

暗通道先驗計算範例：

- (a) 清晰影像
- (b) 算計色彩空間下的三通道 RGB 中最小的數值。
- (c) 套用最小值濾波器來得到區塊中最小的數值。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/2733c794-09a6-403e-a252-12e21de7fe75/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143723Z&X-Amz-Expires=86400&X-Amz-Signature=4b0d2cf223d740591e639690c6dbcd234360c850793f5bb2d7fe8dbb62cc4c00&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

距離影像較近的地方：

銳利且影像飽和度較高，通常是沒有霧霾的存在。

距離影像較遠的地方：

較模糊且因為距離較遠，在光線的傳播上會有衰減，且更容易受到霧霾及懸浮粒子所影響，影像飽和度通常較低。

暗通道先驗之數值一般來講並不會近似於零，在暗通道先驗的
影像中則會看到會偏白不會近似於黑色。

傳輸率會因為距離深度𝑑(𝑥)所影響，在深度𝑑(𝑥)較大的區域，其傳輸率 t(x,y)會有較小之數值。

在假設清晰影像中暗通道先驗的數值近似於零後，並根據He提出的暗通道先驗的理論，假設局部區域內之距離深度會相等的前提下，將光學散射模型簡化過程如下：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/221ae045-3363-420b-a15a-64b7c60a9877/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143739Z&X-Amz-Expires=86400&X-Amz-Signature=00a701cd4d8ee1f369848b93c3730c2a8fece66e3b0d578eb9db9527263cb430&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/d6390c47-56d6-43fd-bca9-224720acbe86/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143758Z&X-Amz-Expires=86400&X-Amz-Signature=164e46b52bbc2ea75c434afd14f0c995d90d8959b30be55dcc429dd8e62b3102&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

上述之假設若是過度除去霧霾，會導致影像失去深度資訊，使影像變得不自然，因此為了保留部分自然程度，需在公式上乘上一個常數w，如下所示：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/16c07589-9340-4f60-a93a-707928deefd7/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143809Z&X-Amz-Expires=86400&X-Amz-Signature=780f5144907b693c5a64ab3fb107d9dd18f67e70dbb39b4cc89e7b41f5aaf4dc&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

而公式中，還有一個常數大氣光A，其計算方法為：

- 先計算暗通道先驗
- 取其數值中亮度數值最高的前 0.1%的平均值為大氣光之數值。

通過先計算暗通道先驗，可以避免受到環境中的白色物件影響。

將上述之結論套用至實際有霧霾之影像：

- (a) 原圖：具有霧霾。
- (b) 經過暗通道先驗計算後，取得的傳輸率的數值。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/abac317c-04ac-49db-9d57-2d3e10e61b46/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143847Z&X-Amz-Expires=86400&X-Amz-Signature=4439c37ca2248bd94cc61b03ff35962653caf0bcd27f6bbee120b16a0e51f0b3&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

但由上圖傳輸率發現，有區塊的效應之影響，因此仍需校正此張傳輸率影像，在 He[2]等人論文中採用 Soft Matting 的技術來進行細化。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/7f596919-aeab-48b0-b7ff-e3a4f21173c4/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143900Z&X-Amz-Expires=86400&X-Amz-Signature=6181274d05769057af36dd361af60aec848470bbcd172cfa070e1217a284bdd5&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

最後，將公式(2.1)移項並整理，求得清晰的除霧影像 J：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/5dc8ceb1-c6a3-46d2-8527-50e0ffd5c6ef/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143915Z&X-Amz-Expires=86400&X-Amz-Signature=e7c1a48cf4b7e208d9231a851afb00e90cb730007caa7e1ebc7dfa399d3a25df&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

其中𝑡0 = 0.1，目的是防止影像中的雜訊被放大。

## 3 改良式暗通道先驗與多尺度裁切限制直方圖均化之夜間除霧演算法

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/90eac838-2d01-4739-bae5-a4ebd0ba1cd4/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T143941Z&X-Amz-Expires=86400&X-Amz-Signature=443f615434efb59f7b76c67e08af83c43472cd15c6db0d9a113bec90d94c6e7c&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

### 3.1 基於高斯和侵蝕運算的新型暗通道先驗的算法

Original：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/4a55d4a0-807a-4996-9887-5c5b58be4875/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144515Z&X-Amz-Expires=86400&X-Amz-Signature=865868746d2c806cc655181ca77d160718f6e29f057592b4ed6979817cc67505&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

Gaussian：

min ⇒ gaussian ⇒ 平滑 

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/3bd80f67-339a-4308-96d5-cd93ead0a932/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144556Z&X-Amz-Expires=86400&X-Amz-Signature=e6ffe1098b9d01119b828811838e01921be8351a71ca109b1e32101a7b466038&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/1895835d-57e6-433d-9b55-fc312edce844/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144009Z&X-Amz-Expires=86400&X-Amz-Signature=55b7d574ef9819d775ea6212852dcf2f4ac7c1c22ef6cc9d2fc8e53f31d8bcca&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

以高斯為基礎之暗通道需滿足以下條件：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/88621cb4-0cd7-457e-ba36-9fd339d27cfa/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144029Z&X-Amz-Expires=86400&X-Amz-Signature=35be4da1e17c4a7ceac1f6b53eaab09d51ad1beeb6630d0252d331a1cfb60891&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

高斯和侵蝕運算(kernel size=5x5)為基礎的暗通道：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/a1971f5c-513f-4106-ab28-f092c4d0f8db/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144043Z&X-Amz-Expires=86400&X-Amz-Signature=7d777643058031840d6cdee4b42503dde8973411cccbfcd5af274f7edc59fd4c&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

### 3.2 經細化及侵蝕運算的傳輸率

Original：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/50125b6c-da17-4d01-974c-ad7189188738/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144638Z&X-Amz-Expires=86400&X-Amz-Signature=c1d648904888ae712d27e6d45e5baba2095eebde847c4a2b7710ede0a64c687f&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

修改大氣光𝐴的計算公式(先計算暗通道先驗)，其數值變小而導致傳導率變小而產生過度除霧。

為了改善這問題，也對夜間霧霾影像使用了 erode 運算，因此𝐼(𝑥, 𝑦)^𝑐之數值會因此變小，以解決傳導率受大氣光𝐴的影響，並以改良的以高斯及侵蝕運算為基礎的暗通道先驗去計算出大氣光𝐴。

Erode：

- 侵蝕運算子的核心大小設置為 15x15
- 𝜔設置為 0.95 以限制傳輸率不會等於 0

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/0c70e020-348a-4c22-85d5-55b3fab638ec/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144116Z&X-Amz-Expires=86400&X-Amz-Signature=e15f31e19ba6299c67ca24ecac1ea33d81bbb2967d5b88bae557caf032ab6b26&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

引導式濾波器（GF）：去除光暈效果 (Halo Effect)

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/0ddc84e6-ecfd-48f0-9888-6e1f0536c024/20A30BB3-2142-4C4A-B2E7-BBF372C6414A.jpeg?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144203Z&X-Amz-Expires=86400&X-Amz-Signature=bcd1921e8fbb72a7b6fff4b218c40af3025c16f7e6ae4b73e92acc5a41e7bedf&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%2220A30BB3-2142-4C4A-B2E7-BBF372C6414A.jpeg%22&x-id=GetObject)

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/b5ac3cea-8d04-44b7-a6a2-92593822bbf1/A13655CF-AA51-4EFB-95A1-AFDB7888A9BA.jpeg?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144214Z&X-Amz-Expires=86400&X-Amz-Signature=4e5308672ae304c7da36915f4f51a3e98c2771763b343767c5e5644ce6fce8c8&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22A13655CF-AA51-4EFB-95A1-AFDB7888A9BA.jpeg%22&x-id=GetObject)

主要用於細化圖像，減少圖像中的雜訊並保存圖像邊緣。

- 𝑠𝑥與𝑐𝑥表示於𝜔𝑘中的常數
- 𝑛(𝑥)為雜訊
- 𝐼(𝑥)表示原始輸入影像

光暈效果：主要發生在圖像邊緣，且數值變化大的區域。

### 3.3 利用多尺度引導式濾波器對傳輸率進行細化之動作

引導式濾波器是一種能使影像平滑化的非線性濾波器，與雙邊濾波器相同，其優點在於：

- 此在計算複雜度上相對的低，也就是使用上比起雙邊濾波器快速許多。
- 雙邊濾波器因為數學模型的緣故，會發生梯度反轉(Gradient Reverse)之情形，導致失真，而引導式濾波器的數學模型是以線性模型為基礎，輸出並與輸入擁有相同的梯度方向。

作法為：

- 對傳輸率t使用不同半徑r的引導式濾波器
    - r=30
    - r=50
    - r=70
    - r=100
- 將得到Ｋ組的結果，使用min運算子找出最終的細化結果。

在此使用min運算子而不是max運算子，是因為若是使用max運算子則會導致傳輸率的數值較大，使得整體除霧效果較弱。

在5 次的多尺度次數，有一個最好的除霧結果下，比起次數 6 及 7 次時間運算複雜度會較少。

### 3.4 介紹多尺度裁切限制的積分對比限制自適應直方圖等化(ICLAHE)的原理及其使用後之效果

多尺度裁切限制的積分對比限制自適應直方圖等化(Multiscale Clipping Limits of ICLAHE)

考量受到霧霾影響的影像特性，常有大量像素值相同的區域出現，若同樣給予強化，將造成除霧影像極其不自然之效應。

在採用CLAHE來限制特定區域的強化斜率，但考量運算複雜的情況下，傳統的CLAHE會降低整體系統的使用性，針對ICLAHE演算法進行改良，提出了多尺度裁切限制的新穎ICLAHE演算法。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/59c4381b-9bda-4997-8bb1-cb563d29ff00/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144237Z&X-Amz-Expires=86400&X-Amz-Signature=d0f69f21f7aac0cf8d1dcee796370bad7330ed76a2df1a8b5193b437f91f98af&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

### CLAHE

CLAHE 的實現過程與 AHE 類似，只是多了裁切與重新分配 PDF 的步驟。

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/ff53c1d4-2839-41d9-b140-9bf7dfd25ff1/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144249Z&X-Amz-Expires=86400&X-Amz-Signature=161d1e46e0e689904b642d79c2d8866a241d15f1663631c62d0b5b0643847f26&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

機率密度函數𝑝(∙)：

- 𝑔 ∈ [0, L]代表像素值是從 0 到 255
- |R𝑖,𝑗| = 𝑀^2則代表著區塊的大小
- R𝑖,𝑗 = {𝑥𝑖+𝑚,𝑗+𝑛 | |𝑚| ≤ |𝑀/2|, |𝑛| ≤ |𝑀/2|}則代表一組中心點位於(𝑖,𝑗)且大小為𝑀 × 𝑀的區塊大小的像素值集合

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/64190351-3cd0-48df-8d06-24df3f0ce8e5/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144259Z&X-Amz-Expires=86400&X-Amz-Signature=52fb37a780a051ec16790e00ed21dcfaeb2b7b218403f87fe9af9322048469ee&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

加入裁切限制𝛾：

- 𝛾 ∈ [0,1]代表裁切限制
- 𝐷描述裁切區域的面積總和(藍色區塊)

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/ee28fb04-2c2d-4a3a-bf5f-e9d48f4309da/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144313Z&X-Amz-Expires=86400&X-Amz-Signature=d2a537e4d22b80558066a6203e64a6228fec2bd11c0bac2bbd0d54d441ec7204&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

重新平均分配裁切區塊後的累積分配函數(CDF)：

![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/d6478a5b-ad85-4f07-82b0-0485dc12ad66/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144328Z&X-Amz-Expires=86400&X-Amz-Signature=fb6f3ced6322337f6518a1cbfdc6c1f3816b8ab48cd19edaddd93038062cc9ef&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)

最後輸入影像與強化結果影像之間的關係被定義為：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F5db72f2f-ccd8-401d-94c2-1ecdf64bac22%2FUntitled.png?table=block&id=e37679b6-bd86-450d-a28f-687307976cf9&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### ICLAHE

一般形式的 CLAHE 是以點基礎(pixel-based)去處理，ICLAHE則使用積分影像來加速 CLAHE 並簡化 PDF。

傳統方法要計算的R𝑖,𝑗周圍的八個鄰點的資訊，具有較高的計算複雜度並導致較慢的執行速度。

- 另一種方法計算，機率密度函式(PDF)的計算可以被認為是每個像素值𝑔的單獨集合的運算，其公式定義如下：
    
    ![image](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/c0debab9-4c7b-4a1c-8993-e6af33155b79/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220626%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220626T144957Z&X-Amz-Expires=86400&X-Amz-Signature=83c049456e2ec509851dcfe278dfbc15c1f8b9b64fb606bc1a562259d1f8ee4f&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22Untitled.png%22&x-id=GetObject)
    
    - 1𝑔(∙)代表集合𝑔的指標函數(indicator function)
    - 𝑥𝑖+𝑚,𝑗+𝑛 ∈ R𝑖,𝑗代表著是其像素點的位置是在其參考區塊的範圍內。
    - 著色的區塊中的大小為M^2

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F721da53a-376a-4ca6-8b0f-fbf736597c56%2FUntitled.png?table=block&id=807f52e3-5d99-40c7-a4c2-fdfba25abe1f&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑣(𝑔)明顯呈現二次方的時間複雜度，其定義為：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F11c3cf32-6d94-48d2-9135-42d9461a78d4%2FUntitled.png?table=block&id=9d947111-a750-4b7a-9cc3-d15d1f169c00&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

其繁重的計算複雜度的原因：

- 大量的數值累計加總
- 每個像素值周圍八鄰點的計算
- 可藉由一階的積分影像進行化簡：
    
    ![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Ff9f3a83c-0ae3-49a2-bb99-2892033ff7da%2FUntitled.png?table=block&id=0a7b5b43-62e9-48e5-9348-c842c305af67&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    
    有效利用先前所取得之積分圖結果來避免重複計算：
    
    ![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F5a818992-1cc0-4737-b3a1-8ad23c9a720d%2FUntitled.png?table=block&id=5b79e944-6126-4410-8d1a-d4dbe59c9e40&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    

藉此化簡𝑣(𝑔)的時間複雜度⇒常數項𝑂(1)：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F61a107f2-34e1-48fe-9c3b-92b40299657f%2FUntitled.png?table=block&id=8326615e-50f7-46cd-9c06-03f8b93fda69&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F3a298910-8be4-4fea-b7cd-5a161ed96ebc%2FUntitled.png?table=block&id=fc687046-b6e6-4a4e-8fdf-323d821b9656&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fcd1ecbe5-ed1e-45d3-88e1-af91c1c621a5%2FUntitled.png?table=block&id=fa892f43-3307-4c91-9f3b-6cb4ffa918b5&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### 轉換函式(Transformation Function)

防止對比度過度增強，雜訊過於放大而產生人工偽像，藉由限制機率密度函式(PDF)的過高的部分進行裁切，也就是下方公式中的𝑣(𝑔)，裁切限制的公式如下：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fbbf28205-7e8a-4032-87ec-6be073e828fe%2FUntitled.png?table=block&id=958861ca-b78d-4466-ad73-8afae7271417&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑣′(𝑔) + 𝐷這一項則是代表補償後的經裁切的機率密度函式(PDF)。

為了得到相應的累進分布函式(CDF)，需要對 PDF 的計算公式(3.13)中的 𝐷/(𝐿+1)這一項進行額外的運算。

對𝑣′(𝑔)和𝐷的上述補償計算數學式 ⇒轉換函式c(𝑔)和f(x)：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fd13e2d4f-b501-4bd6-83f2-ad16fed09487%2FUntitled.png?table=block&id=11030464-bdac-4205-aae0-6092de92ba3a&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

針對𝑣′(𝑔)的 PDF 中每一個灰階值進行 𝐷/(𝐿+1)的補償運算，會造成運算量大幅度的增加，可以直接被平移到轉換函式中。

- 𝐿/|𝑅𝑖,j| 為常數項

可藉由線性累加的方式 𝐷/(𝐿+1)×(𝑥𝑖,𝑗 + 1)推算出𝑣′(𝑔)的 CDF 需要補償的面積，藉此避免針對𝑣′(𝑔)中所有灰階值的正規化和補償運算。

- 計算範圍被修改在[0, 𝑥𝑖,𝑗]之間而不是先前的[0, 𝐿]

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F631c794c-5d2e-4f2e-9800-a8bf9c569bda%2FUntitled.png?table=block&id=bd747c28-8272-4097-a708-ee0e42256e82&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### 多尺度ICLAHE

多尺度裁切限制

𝑘個不同的機率密度函式(PDF)：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F7119836d-187e-4971-86e2-0702a1c53577%2FUntitled.png?table=block&id=e2c4a30e-f347-4f14-b330-99e55862332f&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑟𝑘′表示𝑘個不同的裁切限制的閾值

得到相對應的𝑘個不同的累進分布函式(CDF)：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fb971fbd7-2bee-4111-927a-689b761fb9be%2FUntitled.png?table=block&id=e03ca234-fe09-4102-8920-5cd4032d4c97&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑘個不同的轉換函式：

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F3af270a9-7565-4c7f-afba-186123aeaa11%2FUntitled.png?table=block&id=9d884a5b-1b11-4b4e-ac62-9124cb0256d0&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑑𝑠𝑡(𝑥𝑖,𝑗)代表最終夜間除霧結果

- (a) 原圖
- (b) ICLAHE 的固定式裁切限制
- (c) ICLAHE 的多尺度裁切限制

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fdb8075d0-3115-4956-a496-dd6ec5f840a6%2FUntitled.png?table=block&id=c68499ea-1b30-4b97-b8b2-017f1bb1ce05&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

在使用次數 6 次的多尺度次數後會有較好的除霧結果

### 3.5 比較我們提出的夜間除霧演算法所產生的夜間除霧影像和其他最新的夜間除霧演算法的結果

- 定性評估(Qualitative Assessment)
    - 色彩偏移
    - 在天空的部分有嚴重的雜訊及人工偽影
- 主觀用戶研究(Subjective User Study)
    
    主觀用戶研究的評估和計算方法來獲得主觀 IQA 評分
    
    - 50 個影像集合
    - 50 名民眾 (32 名男性和 18 名女性)
    - 1 到 10 的整數進行評分
    
    ![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F6465d24c-621a-4f0f-9465-89bfbd235fac%2FUntitled.png?table=block&id=1141133d-5612-497e-b16c-2f0178a5b5a4&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    
- 時間複雜度(Computation Complexity)
    
    ![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F7fa8aef5-ba0c-40ef-9dcc-dc6e0f82287c%2FUntitled.png?table=block&id=eb94f84a-c499-458a-918f-74a051a2e767&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    
- 夜間除霧結果(Nighttime Dehazing Result)
    
    將各自的夜間除霧結果進行比較
    
    - 色彩偏移
    - 過度除霧而導致過暗
    - 雜訊點被放大而產生人工偽影