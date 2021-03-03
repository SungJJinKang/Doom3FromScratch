#include "JobSystem.h"

#include <limits>

#include "../Core.h"
#include "Thread.h"

using namespace doom::resource;

void JobSystem::Init()
{
	this->InitializeThreads();
}

void JobSystem::Update()
{

}

void doom::resource::JobSystem::OnEndOfFrame()
{
}

doom::resource::Thread& JobSystem::GetSleepingThread() const
{
	doom::resource::Thread* fewestWaitingThread{ &(this->mManagedSubThreads[0]) };
	size_t fewestWaitingJobCount = std::numeric_limits<size_t>::max();

	for (unsigned int i = 0; i < SUB_THREAD_COUNT; i++)
	{
		if (this->mManagedSubThreads[i].GetIsThreadSleeping() == true)
		{
			return this->mManagedSubThreads[i];
		}
		else if (size_t waitingJobCount = this->mManagedSubThreads[i].GetWaitingJobCount() < fewestWaitingJobCount)
		{
			fewestWaitingThread = &this->mManagedSubThreads[i];
			fewestWaitingJobCount = waitingJobCount;
		}
	}

	return *fewestWaitingThread;
}

void JobSystem::WakeUpAllThreads()
{
	for (unsigned int i = 0; i < SUB_THREAD_COUNT; i++)
	{
		if (this->mManagedSubThreads[i].GetIsThreadSleeping() == true)
		{
			this->mManagedSubThreads[i].PushBackJob(std::function<void()>()); // push dummy job
		}
	}
}

void JobSystem::InitializeThreads()
{
	this->mMainThreadId = std::this_thread::get_id();

	this->mManagedSubThreads = std::make_unique<Thread[]>(SUB_THREAD_COUNT);
	

	for (unsigned int i = 0; i < SUB_THREAD_COUNT; i++)
	{
		this->mManagedSubThreads[i].SetPriorityWaitingTaskQueue(&(this->mPriorityWaitingTaskQueue));
	}

	this->bmIsInitialized = true;
}

void JobSystem::DestroyThreads()
{
	for (unsigned int i = 0; i < SUB_THREAD_COUNT; i++)
	{
		this->mManagedSubThreads[i].TerminateThread(true);
	}
	this->bmIsInitialized = false;
}

Thread& doom::resource::JobSystem::GetThread(size_t threadIndex)
{
	D_ASSERT(this->bmIsInitialized == true);
	D_ASSERT(threadIndex >= 0 && threadIndex < SUB_THREAD_COUNT);
	return this->mManagedSubThreads[threadIndex];
}

std::thread::id JobSystem::GetMainThreadID() const
{
	return this->mMainThreadId;
}

JobSystem::~JobSystem()
{
	this->DestroyThreads();
}