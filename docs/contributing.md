# Guía de Contribución

## Añadir una Nueva Librería

### 1. Estructura de Carpetas

Cada librería debe seguir esta estructura:

```
libraries/NombreLibreria/
├── src/
│   ├── NombreLibreria.h
│   └── NombreLibreria.cpp
├── examples/
│   └── BasicUsage/
│       └── BasicUsage.ino
├── keywords.txt
├── library.properties
└── README.md
```

### 2. Archivo library.properties

```properties
name=NombreLibreria
version=1.0.0
author=Tu Nombre <email@example.com>
maintainer=Tu Nombre <email@example.com>
sentence=Descripción breve de la librería
paragraph=Descripción detallada de funcionalidades y características principales
category=Timing
url=https://github.com/Jusaba/ESP32-Jusaba-Librerias
architectures=esp32
depends=WiFi
```

### 3. Archivo keywords.txt

```
#######################################
# Datatypes (KEYWORD1)
#######################################

MiClase	KEYWORD1

#######################################
# Methods and Functions (KEYWORD2)
#######################################

miMetodo	KEYWORD2

#######################################
# Constants (LITERAL1)
#######################################

MI_CONSTANTE	LITERAL1
```

### 4. README.md de la Librería

Debe incluir:
- ✨ Características principales
- 📦 Instalación
- 🚀 Ejemplo de uso rápido
- 📖 Referencia completa de API
- ⚙️ Configuración (si aplica)
- 🔧 Troubleshooting común

### 5. Documentación del Código

Usar comentarios Doxygen para todas las funciones públicas.

## Estándares de Código

### Nomenclatura
- **Clases**: PascalCase (`MiClase`)
- **Métodos**: camelCase (`miMetodo()`)
- **Constantes**: UPPER_SNAKE_CASE (`MI_CONSTANTE`)

### Formato
- Indentación: 4 espacios
- Máximo 100 caracteres por línea

## Contacto

¿Preguntas? Abre un [issue](https://github.com/Jusaba/ESP32-Jusaba-Librerias/issues).
