%% Ler matriz A.
b = load("matrixA_30x20.txt");
%%

%% Ler matriz B.
a = load("matrixB_30x20.txt");
%%

%% Display da primeira imagem.
figure(1)
%%

%% Passar os pixeis para uma escala de 0 a 1
b=b/2;
%%

imshow(b);
%% Passar os pixeis para uma escala de 0 a 1
a = a/2;
%%

%% Os pixeis que originalmente tinham 0 ficam a preto, 
%% os que tinham 1 ficam a cinza e os que tinham 2 
%% ficam a branco
figure(2)
%%
imshow(a);
