# **畸變影像校正與樣板比對加速技術**

# Motivation

畸變影像校正的目的：

在於將因透鏡產生畸變的成像回復到理想中非畸變之影像
影像的品質好壞將會影響後續影像處理相關應用

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F6585a272-09f0-496c-b4d5-ed7cc5a54e08%2F51E5690B-FBA6-4090-870F-31E2AF1CA5D1.jpeg?table=block&id=1004ddcf-2ff0-4f4e-97d7-a0d86b2f93ac&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

# Methods

現今：

藉由實際物體的資訊（如棋盤格之間的距離與成像之間的關係）
計算出相機參數

目標：

只輸入單一畸變影像，利用輪廓校正前、後產生的彎曲變化，進行迭代計算，求出畸變參數與相機參數。

HALCON：

已提供畸變影像校正技術相關的函數
本計劃將以Ｃ語言實現並加速計算過程

### 演算法流程

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F666d3db6-242b-4a1a-be5b-2501166d91a0%2F021EEDC2-6B20-403A-ABB5-E3B0F7990948.jpeg?table=block&id=0464d790-aeec-4b10-8861-868507756141&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

畸變模型： Radial Distortion 以及 Tangential Distortion

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F99ffc3de-b94a-4dd8-9f0e-3e53b0af721d%2F05EB3E18-5D6F-4018-9B67-2D165E07D472.jpeg?table=block&id=6a3ad0c2-f727-4e88-841d-7198e50957bb&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝐿(𝑟)為畸變模型：polynomial 以及 division

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F05cb286e-4416-42b6-a591-3886a334d924%2FD6AA25A6-E17B-40FD-A5E6-0383EF9D1FE2.jpeg?table=block&id=8ad6bd64-02cd-4492-9699-dabf8d7d364d&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fce1ac976-7113-4a73-b08b-dc151d2cebff%2F4BD0FA3D-9F0F-40C5-B06C-D3FE1D81116D.jpeg?table=block&id=483298ec-fa50-4871-ac90-3056748ee453&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

## 2.1 Edge detection

### Canny⇒輪廓點

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Faee74f71-bae4-49b8-9830-fb2e5006d34c%2F6D7EFC46-9638-4545-AF52-6D9A4F073B46.jpeg?table=block&id=74058ba5-807b-461e-b3b5-cbe85d33522c&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

high threshold：前20%

low threshold：後30%

### Clean⇒刪除較不重要的點

5*5 的 window 內皆屬於 neighborhood points，並將每個 neighborhood points 與自己的 orientation 作內積，所有內積值累加存為此點的scalar_product。

條件（二其一 ⇒ 刪除此點）：

1. neighborhood points 個數小於 2
2. scalar_product 小於此點的 neighborhood points 個數乘以 0.95

上述重複4次

1. 5*5 的 window⇒max() 
    
    window 步伐為 window 大小，因此不會有重疊的問題。
    

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F882cd53d-4d44-4bef-a13f-0e12e7926739%2F5DE382F4-6485-4ADC-B3A0-B478C8613E05.jpeg?table=block&id=08fec0db-a11f-4e6b-95e4-712486fbc18b&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

## 2.2 Detection of lines with the improved Hough transform

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fc78bc42b-2d1f-4323-a541-3784b2c098e0%2F246CC063-1E94-456F-8D70-FB0394B26C34.jpeg?table=block&id=ea1ccfdd-d60b-4188-91e3-c5436bab275d&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

傳統：

Hough transform無法判斷彎曲的輪廓，導致失真。

Improved：

將畸變作為傳統二維霍夫變換的第三個維度⇒到原點的距離、方向與畸變
透過迭代計算畸變參數⇒求出k1⇒再求出k2  
計算過程中k1、k2將以p1、p2（校正百分比）進行變數轉化

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F7485625b-a9b8-44bd-af00-b8583ccf9194%2FA548479F-C10C-4B9D-B99F-F8F8729E42C4.jpeg?table=block&id=b855ae61-4113-495e-a03e-545250396ccf&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fe41f44ca-e562-4d29-a42f-1d0c70f293bb%2FC24B4B4E-2238-46CA-9F46-153044DB5B16.jpeg?table=block&id=f6ed4dc1-3e6c-49cf-8b1f-b46090b05289&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### Hough transform

霍夫轉換是影像處理中的一種特徵提取技術，透過檢測累積結果的區域性極值點，進行投票取得一個符合特定形狀的點的集合。

一個點可以為無限多條線的焦點，轉換到rho,theta座標後，會變成一條線。將多個點的rho,theta上的線描繪出來，其焦點為點與點之間的直線。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fa332302d-e455-4a66-b37d-7edf52df0028%2F7FB49C26-FF71-49C0-9920-E6F975D91CA1.jpeg?table=block&id=e4d28165-2200-4dc3-9b34-9f63bd853547&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

避免出現斜率無窮大的情況，採用極座標代替點斜式

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fc3bee371-9f47-4c3a-b564-5fb51606059e%2F496A42F1-DB6C-40F1-ACEB-6D1D5373676C.jpeg?table=block&id=e4225911-72ad-475a-928d-eec1a41ae18f&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

針對n個不同的k1，進行n次的輪廓校正，後並進行霍夫轉換的投票，取得數個𝑑、𝜃的投票結果。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F035963ff-3350-4c07-873e-eb8c35f58c3c%2F93983796-8AC1-4E19-9B32-C805481DCE81.jpeg?table=block&id=02d80c20-6d97-411c-8c65-13539b4b39b2&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### Maximum selection

根據當下的k1的數個𝑑、𝜃投票結果，選出票數最多的前Ｎ條的線
（Ｎ＝100），並將Ｎ條線的票數加總作為此k1的vote score。

根據最高的vote score，選出最佳的k1。

### Capturing points

若有輪廓點沒有被歸到任何一條線中，即捨棄此輪廓點

歸類須符合條件：

1. 內積值要大於閥值
2. 點到線的距離要小於閥值

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fcd769e74-8b32-4e16-a02f-af8b00172de2%2F3913248E-C1E3-4CEC-B21A-4C86FF18B418.jpeg?table=block&id=39697ef8-7def-499e-bff9-937137c8eb3e&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### Ensure consistent line orientation

透過線上的每個點與此線的方向進行內積，將少數方向的點移除。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F811307bc-b295-4cc3-b2dd-005a0532a6f8%2FF3B6090E-6BAE-4EAF-9950-B6617BAC30F4.jpeg?table=block&id=c057b382-204c-4723-a293-a7af1b74c4bb&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### Recompute line equations

將剩下的點，重新計算出直線方程式，透過
最小平方法-垂直偏移(Least Squares Fitting--Perpendicular Offsets)
求出直線方程式𝑦 = 𝑎 + 𝑏𝑥的係數𝑎與𝑏

1. 將點到線的距離平方和分別對a、b進行偏微分
2. 透過minimum = 0 的等式⇒求出b⇒再求出a

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F1c5a8ea8-225e-4c6e-a56f-5f91490e60d5%2FFB966B09-82B3-4212-A46C-738C4F29FF1D.jpeg?table=block&id=a8a391cd-7e8f-45c4-abb8-b86bf9a604a2&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### Remove and join

N 條線中是否有需要刪除的線或是將兩條線合併成一條線，進行 N 條直線的整併。
合併後再次進行Recompute line equations。

## 2.3 Iterative optimization

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F792e5aff-720d-4d83-a93c-b2961cb28e70%2F0ED44B11-07F9-49A5-8D28-7C9849C9C780.jpeg?table=block&id=4e3b51e7-de56-4a84-9691-4e8cfcbf7c3c&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

假設點到線距離平方總和為 Energy，d 代表某一組(𝑘1, 𝑘2, 𝑥𝑐, 𝑦𝑐)⇒(p1, p2, 𝑥𝑐, 𝑦𝑐)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2Fea1cac14-0490-4f22-8f21-db9cfa57d3bd%2FA5B90F8A-4859-4C6C-9629-92B73788E672.jpeg?table=block&id=fd354f29-25c0-4a8b-a7c5-02a75f041df6&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

𝑘1初始值為第二部分估算出的數值，𝑘2初始值為 0
為使𝐸最小化，針對d的四個參數進行導數計算：

∇𝐸(𝑑0)的一階導數和二階導數分別為 

- gradient
    
![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F3c0fd13a-7ce0-48ef-a017-008da2dcd2bb%2FUntitled.png?table=block&id=d913b0a5-ba66-4280-abab-0ff01e3ad82e&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    
- hessian 矩陣
    
![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F08f66c74-6a58-4ddb-a8e2-3b7515ad4cd6%2FUntitled.png?table=block&id=9ab12286-d646-46a2-b587-d77c4d8ba7b4&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)
    

ℎ1、ℎ2分別為迭代畸變參數及畸變中心點時增加的步伐
預設為ℎ1 = 10^−4、ℎ2 = 10^−2。

1. 每次迴圈都會更新𝑑𝑛𝑒𝑥𝑡 = 𝑑 + 𝑔𝑟𝑎𝑑𝐸
2. 更新後的(𝑝1、𝑝2)可以經由轉換公式得到更新後的(𝑘1、𝑘2)
3. 由新的畸變參數以及畸變中心點 校正輪廓
4. 計算此k1的vote score

迭代條件：

1. vote score有大於上一個迴圈算出的 1.01%，則繼續進行迭代
2. 小於算一次失敗，累積三次以上將會停止迭代

求出擁有最高vote score的參數(𝑘1, 𝑘2, 𝑥𝑐, 𝑦𝑐)。

## 2.4 Correction of image distortion

根據參數(𝑘1, 𝑘2, 𝑥𝑐, 𝑦𝑐)取得校正影像

1. 透過牛頓法找出inverse function
2. 使用雙線性內插法還原影像

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F77f12fa8-5f64-4736-9f81-91921622b878%2F82759B64-7BE0-4BCD-82D7-35079C7956E3.jpeg?table=block&id=cfefc6d6-8a97-4458-83be-453970aedc31&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### 校正座標

在回查座標點時，座標會乘上一個 scale 參數，將圖像頂點定位。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F8a6c56c9-e777-4154-8df1-a9b556482c67%2FUntitled.png?table=block&id=393bfb78-e5a1-4c95-9055-a5a1d1105dca&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F943fc0c8-23f1-4f05-8572-124582cb6949%2FUntitled.png?table=block&id=a0c6fbc6-073e-4760-8b22-3aeede31320c&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

### 最大有效區切割

1. 校正前會先建構一個原圖同等大小的影像作為底圖
2. 經過校正後，若 scale 值大於 1，會產生無法回查的位置⇒填入0⇒產生無效區

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F552eaa8a-c58d-4026-bbff-e6f416db87ea%2FUntitled.png?table=block&id=1e335ac2-6cd7-4a22-bdf4-b296b0ba3b92&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

透過找出四邊的最大的 8 連通元件，切割出最大的有效區。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F8dacbbb0-9785-404c-a94d-861639e97797%2FUntitled.png?table=block&id=f8673512-bd44-46c3-bd96-50493311c31c&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

## 2.5 SSE

SSE（Streaming SIMD Extensions）是一種 SIMD 指令集，也就是一個指令可以同時對多個資料進行相同的動作。
SSE 新增了八個新的 128 位元暫存器（xmm0 ~ xmm7），可以用來存放四個 32 位元的單精確度浮點數。

SSE 的浮點數運算指令，大致上可以分成兩種：

1. packed：四個浮點數 DATA0 ~ DATA3 進行計算
2. scalar：只對一個浮點數 DATA0 進行計算

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F3cd36ac5-affc-4af0-9987-df0ef8868be9%2FUntitled.png?table=block&id=a611fd1c-fc28-43f1-8389-045e8e36f11c&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)

將函式中於迴圈內的運算部分利用 SSE 指令改寫，可以一次處理四個變數，相當於此迴圈執行次數可以減為原本的 1/4。

![image](https://www.notion.so/image/https%3A%2F%2Fs3-us-west-2.amazonaws.com%2Fsecure.notion-static.com%2F878f6763-3b3c-4ce3-9f7e-369a58dfd344%2FUntitled.png?table=block&id=43d581b8-04fc-4dae-83b1-60921040c5fc&spaceId=8900ffca-07f1-4e23-9c4e-9d97b247dcd6&width=1390&userId=eebbf37a-c0f3-48f8-8e2a-eeaf65e50166&cache=v2)