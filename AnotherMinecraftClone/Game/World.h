#pragma once

#include <inttypes.h>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>

#include "glm/glm/vec3.hpp"
#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/mat4x4.hpp"

struct GLFWwindow;

namespace Game
{
	#define CHUNKWIDTH 32
	#define CHUNKHEIGHT 350
	#define BLOCKCOUNT ((CHUNKWIDTH * CHUNKWIDTH) * CHUNKHEIGHT)

	#define RENDERDISTANCE (10 * CHUNKWIDTH)

	//This is probably horrible.
	#define BLOCKINDEX (x + CHUNKWIDTH * (y + CHUNKHEIGHT * z))

	class Controller;

	namespace Graphics
	{
		class RenderMesh;
		class RenderShader;
	}

	namespace MathHelpers
	{
		# define M_PI 3.14159265358979323846

		float DegToRad(float deg);
		glm::quat GetAxisAngle(glm::vec3 Direction, float Angle);

		glm::mat4 Translate(glm::vec3 Translation);
		glm::mat4 Rotate(glm::quat Rotation);
		glm::mat4 Scale(glm::vec3 Scale);
		glm::vec3 GetForward(glm::mat4 Transform);
		glm::vec3 GetRight(glm::mat4 Transform);
	}

	struct Block
	{
	public:

		uint8_t ID = 0;
	};

	class Chunk;
	class World;

	class Chunk
	{
	private:

		Graphics::RenderMesh* Mesh;

		bool DataGenerated = false;
		bool MeshGenerated = false;
		bool ReadyForRendering();

		void GenerateMeshForBlock(int x, int y, int z);
		void GenerateFaceIfNeeded(int x, int y, int z, int ox, int oy, int oz);
		void AddQuad(bool Reverse = false);
		void SetNormals(glm::vec3 Direction);

		std::vector<glm::ivec3> BlockQue;

	public:

		uint64_t Key;

		static void LoadChunkShader();
		static Graphics::RenderShader* ChunkShader;

		int _PositionX;
		int _PositionZ;

		World* world;

		Block BlockBuffer[BLOCKCOUNT];

		bool OtherChunkReady(int X, int Z);

		void GenerateChunk();
		void GenerateData();
		void GenerateMesh();

		Block GetBlock(int x, int y, int z);

		//If you call this, this chunk should be deleted :(
		void TestGenerate();

		void Render();

		Chunk() { throw 0; };
		Chunk(int X, int Z, World* world);
		~Chunk();
	};
	
	class Camera
	{
	public:

		float fov = 60;

		glm::vec3 Position				= { 0,100,0 };
		glm::vec3 EularAngleRotation	= { 0,0,0 };

		glm::vec3 Forward;

		glm::mat4 GetRotationMatrix();
		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetTransformMatrix();

		glm::mat4 GetViewMatrix();

		void Update(Controller* controller, float DeltaTime);
	};

	class World
	{
	private:

		std::chrono::steady_clock FrameCounter;
		std::chrono::steady_clock::time_point Last;

		void SubmitFrameTime();

		double FrameRate;

		static float DeltaTime;

		int WindowWidth;
		int WindowHeight;

		std::map<uint64_t, Chunk*> ChunkMap;
		std::vector<Chunk*> ChunkCreationQue;
		std::vector<Chunk*> ChunkDestructionQue;

	public:

		GLFWwindow* WorldWindow;

		Controller* MainController;

		Camera MainCamera;

		static World* MainWorld;

		void Run();
		void UpdateWorld();

		//Chunk Code
		std::mutex DispatchLock;
		bool ReadyToDispatch = true;

		uint64_t GetKey(uint64_t X, uint64_t Z);
		bool ChunkExists(uint32_t X, uint32_t Z);
		Chunk* GetChunk(uint32_t X, uint32_t Z);
		void CreateChunk(uint32_t X, uint32_t Z);
		bool TestForChunkDestruction(Chunk* chunk);
		void InsertChunkToCreationQue(Chunk* chunk);
		void GenerateAllChunks();
		void RenderAllChunks();

		World();
		~World();
	};
}