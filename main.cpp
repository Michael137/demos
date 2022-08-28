#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <string>

void swapBuffers( SDL_Window* win ) { SDL_GL_SwapWindow( win ); }
void clearWindow()
{
    glClearColor( 0.0f, 0.15f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

bool updateWindow( SDL_Window* win )
{
    swapBuffers( win );

    SDL_Event e;

    while( SDL_PollEvent( &e ) ) {
        if( e.type == SDL_QUIT ) {
            return true;
        }
    }

    return false;
}

static std::string LoadShader( std::string const& filename )
{
    std::ifstream file{ filename };

    std::string output;
    std::string line;
    if( file.is_open() ) {
        while( file.good() ) {
            std::getline( file, line );
            output += line;
            output += '\n';
        }
    } else {
        std::cerr << "Unable to load shader: " << filename << std::endl;
    }

    return output;
}

static void CheckShaderError( GLuint shader, GLuint flag, bool isProgram,
                              std::string const& errorMessage )
{
    GLint success      = 0;
    GLchar error[1024] = { 0 };

    if( isProgram )
        glGetProgramiv( shader, flag, &success );
    else
        glGetShaderiv( shader, flag, &success );

    if( success == GL_FALSE ) {
        if( isProgram )
            glGetProgramInfoLog( shader, sizeof( error ), NULL, error );
        else
            glGetShaderInfoLog( shader, sizeof( error ), NULL, error );

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

static GLuint CreateShader( std::string const& text, GLenum shaderType )
{
    GLuint shader = glCreateShader( shaderType );
    if( shader == 0 ) {
        std::cerr << "Error: Shader creation failed!" << std::endl;
    }

    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    shaderSourceStrings[0]       = text.c_str();
    shaderSourceStringLengths[0] = text.length();

    glShaderSource( shader, 1, shaderSourceStrings, shaderSourceStringLengths );
    glCompileShader( shader );
    CheckShaderError( shader, GL_COMPILE_STATUS, false,
                      "Error: Shader compilation failed!" );

    return shader;
}

struct Shader
{
    Shader( std::string const& fileName )
    {
        // Allocate space on the GPU for our program
        m_program = glCreateProgram();

        // Shader (i.e., function over each pixel) that creates mesh
        // coordinates
        m_shaders[0] =
            CreateShader( LoadShader( fileName + ".vs" ), GL_VERTEX_SHADER );

        // Shader that adds color to meshes
        m_shaders[1] =
            CreateShader( LoadShader( fileName + ".fs" ), GL_FRAGMENT_SHADER );

        for( uint64_t i = 0; i < kNumShaders; ++i ) {
            glAttachShader( m_program, m_shaders[i] );
        }

        glBindAttribLocation( m_program, 0, "position" );

        glLinkProgram( m_program );
        CheckShaderError( m_program, GL_LINK_STATUS, true,
                          "Error: Program linking failed!" );

        glValidateProgram( m_program );
        CheckShaderError( m_program, GL_VALIDATE_STATUS, true,
                          "Error: Program is invalid!" );
    }

    // Make GPU use shader functions tied to this
    // Shader object
    void bind() { glUseProgram( m_program ); }

    ~Shader()
    {
        for( uint64_t i = 0; i < kNumShaders; ++i ) {
            glDetachShader( m_program, m_shaders[i] );
            glDeleteShader( m_shaders[i] );
        }
        glDeleteProgram( m_program );
    }

    static constexpr uint64_t kNumShaders = 2;
    GLuint m_program;
    GLuint m_shaders[kNumShaders];
};

int main()
{
    const int width         = 720;
    const int height        = 480;
    const std::string title = "Test Demo";

    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,
                         1 ); // Allocates space for 2 windows

    SDL_Window* win = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, width, height,
                                        SDL_WINDOW_OPENGL );

    SDL_GLContext glContext = SDL_GL_CreateContext( win );

    auto cleanup = [&] {
        SDL_GL_DeleteContext( glContext );
        SDL_DestroyWindow( win );
        SDL_Quit();
    };

    GLenum status = glewInit();
    if( status != GLEW_OK ) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        cleanup();
        return -1;
    }

    // Variables defined below require GLEW to be initialized
    // before creation
    Shader shader( "./res/basicShader" );

    bool closed = false;
    while( !closed ) {
        clearWindow();
        shader.bind();

        closed = updateWindow( win );
    }

    cleanup();
    return 0;
}
