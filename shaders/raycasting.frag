#version 130
varying vec3 vertex, normal;


uniform float level1, step_length;          //изозначение и длина шага луча
uniform vec3 box1,box2;                     //координаты min,max углов ограничивающей коробки (box)

uniform vec3 pos;                           //позиция наблюдателя (камеры)
uniform vec3 nav;                           //направление взгляда наблюдателя (через центр окна Near)
uniform vec3 size;                          //размер массива данных по XYZ
uniform vec3 scale;                         //масштаб данных

uniform sampler3D f_text;                   //текстура для передачи в шейдер исходных данных томограммы


// три вектора (матрица 3х3 преобразования цвета пиксела для левого и правого глаза в стерео-режиме Анаглиф)
uniform vec3 RFrom;
uniform vec3 GFrom;
uniform vec3 BFrom;

const vec3 level_color1 = vec3(0.7,0.8,1.0);        //цвет изоповерхности
const vec3 level_color2 = vec3(1.0,0.8,0.0);        //цвет изоповерхности с обратной стороны
const float surface_opacity = 0.6; //непрозрачность изоповерхности

// используются для вычисления глубины достигнутой лучем при заполнении буфера глубины в связке с OpenGL-растеризацией (Box)
const float z_near=0.3;
const float z_far=60.0;

//	выборка значения данных (скалярного 3D поля) трилинейно интерполированного аппаратно в точке луча
float Equ(in vec3 arg)
{
     return texture3D(f_text, arg/scale).x;
}

//	вычисление нормированного градиента (т.е. нормали к изоповерхности)
vec3 GradEqu(in vec3 arg)
{
    vec3 delta=vec3(1.0)/size;
    vec3 res;
    res.x = Equ(vec3(arg.x+delta.x,arg.y,arg.z))-Equ(vec3(arg.x-delta.x,arg.y,arg.z));
    res.y = Equ(vec3(arg.x,arg.y+delta.y,arg.z))-Equ(vec3(arg.x,arg.y-delta.y,arg.z));
    res.z = Equ(vec3(arg.x,arg.y,arg.z+delta.z))-Equ(vec3(arg.x,arg.y,arg.z-delta.z));
    return normalize(res);
}
// коэффициенты в формуле Фонга
#define K_A 0.1
#define K_D 0.9
#define K_S 0.9
#define P 64.0

// вычисление цвета (освещение по Фонгу) в точке point c нормалью normal и собственного цвета color
vec3 Phong ( vec3 point, vec3 normal, vec3 color)
{
    vec3 light = normalize( pos - point ); // считаем, что источник света всегда в позиции наблюдателя
    float diffuse = dot ( light, normal );
    vec3 reflect = reflect ( -light, normal );  // отраженный луч
    float specular = pow ( max (0.0, dot ( reflect, light ) ), P );  // возведение скалярного произведения в степень
    return K_A * vec3(1.0) + diffuse * ( K_D * color + K_S * specular * vec3(1.0) );
}

// вычисление точки пересечения с коробкой (b1,b2) луча из точки pos с направлением nav. Возвращает t, где точка пересечения - это pos+nav*t
float IntersectBox ( vec3 b1, vec3 b2, vec3 pos, vec3 nav)
{
    vec3 OMAX = ( b1 - pos ) / nav;
    vec3 OMIN = ( b2 - pos ) / nav;
    vec3 MAX = max ( OMAX, OMIN );
    float res = min ( MAX.x, min ( MAX.y, MAX.z ) );
    return res;
}

//цвет на сечении 
vec3 GetLevelColor(float l)
{
    return vec3(0.5,0.5,1.0);
}

// вычисление глубины в точке ps
float GetDepth(vec3 ps)
{
    return z_far/(z_far-z_near) - (z_far*z_near/(z_far-z_near))/(dot(ps-pos,nav));
}


bool IntersectBox ( vec3 Origin,vec3 Direction, vec3 minimum, vec3 maximum, out float start, out float final )
{
    vec3 OMAX = ( minimum - Origin ) / Direction;
    vec3 OMIN = ( maximum - Origin ) / Direction;
    vec3 MAX = max ( OMAX, OMIN );
    vec3 MIN = min ( OMAX, OMIN );
    final = min ( MAX.x, min ( MAX.y, MAX.z ) );
    start = max ( max ( MIN.x, 0.0), max ( MIN.y, MIN.z ) );
    return final > start;
}

void main()
{
// старт луча	
    float op = 1.0,start,final;
    vec3 color = vec3(0.0);                             //... накопленный цвет на старте - черный
    vec3 norm = normal, ray = normalize(vertex-pos);    // нормаль, направление движения вдоль луча
    vec4 step  = vec4(ray*step_length,step_length);     // вектор шага в направлении луча
    vec4 ps = vec4(vertex,0.0);                         // позиция старта луча в интерполируемой вершине near-плоскости

    IntersectBox (ps.xyz,ray,box1,box2,start,final );   // определение на луче положения входа в бокс и выхода из него
    ps = ps + step*(start/step_length);                 // перенос старта на границу бокса


    float e = Equ(ps.xyz),e0;                           //значение поля на текущем и предыдущем шагах
    ps+=step;

            gl_FragDepth = 1.0;                        //по умолчанию глубина максимальная
            for(float ii=0.0;ii<2000.0 && (ps.w<final) && (op>0.03);ii+=0.1)
            {
                e0 = e;
                e = Equ(ps.xyz);                       //вычисляем поле в текущей позиции луча

                vec3 tmp = vec3(level1,e0,e);
                float opp;
                if(clamp(tmp.x,tmp.y,tmp.z) == tmp.x)   // tmp.x=level1=изозначение; попало внутрь [e0,e] на возрастании?
                {
                    vec3 ps11 = ps.xyz + step.xyz*((tmp.x-tmp.z)/(tmp.z-tmp.y));
                    norm = -GradEqu(ps11);

                    opp = dot(ray,norm);
                    opp = 1.0-opp*opp;
                    opp = opp*opp*opp*opp;
                    opp = mix(surface_opacity,1.0,opp);

                    color = mix(color , Phong(ps11,norm,level_color2) , op * opp); 
                    op *= 1.0 - opp;
                    if(gl_FragDepth==1.0) gl_FragDepth = max(GetDepth(ps.xyz-step.xyz),0.1);     //break;
                }else
                if(clamp(tmp.x,tmp.z,tmp.y) == tmp.x)  // на убывании? 
                {
                    vec3 ps11 = ps.xyz + step.xyz*((tmp.x-tmp.z)/(tmp.z-tmp.y));
                    norm = GradEqu(ps11);
                    opp = dot(ray,norm);
                    opp = 1.0-opp*opp;
                    opp = opp*opp*opp*opp;  // подбор степени прозрачности в зависимости от угла луча с нормалью
                    opp = mix(surface_opacity,1.0,opp);
                    color = mix(color , Phong(ps11,norm,level_color1) , op * opp); 
                    op *= 1.0 - opp;
                    if(gl_FragDepth==1.0) gl_FragDepth = max(GetDepth(ps.xyz-step.xyz),0.1);
                    //	break;
                }
                ps+=step;



            }
            
    
    mat3 m_anag = mat3(RFrom,GFrom,BFrom);    // создаём матрицу для анаглифа

    // окончательно определяем цвет пикселя с учетом анаглифа и непрозрачности
    gl_FragColor = vec4( color * m_anag ,1.0-op);
//gl_FragColor = vec4( color ,1.0-op);
}
