#include <windows.h>
#include <fileapi.h>
#include <minwinbase.h>
#include <winbase.h>

#include "fswatch.h"

/** Mostly implemented from nickav's example on ReadChangeDirectoryW
 *  @see https://gist.github.com/nickav/a57009d4fcc3b527ed0f5c9cf30618f8 */
void watch_directory(const char* path_name) {

	DWORD notify_filter =
												FILE_NOTIFY_CHANGE_FILE_NAME
												| FILE_NOTIFY_CHANGE_DIR_NAME
												// | FILE_NOTIFY_CHANGE_ATTRIBUTES
												// | FILE_NOTIFY_CHANGE_SIZE
												| FILE_NOTIFY_CHANGE_LAST_WRITE
												// | FILE_NOTIFY_CHANGE_SECURITY
												;

	HANDLE fs_handle = CreateFileA(path_name,
																	GENERIC_READ,
																	FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
																	NULL,
																	OPEN_EXISTING,
																	FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
																	NULL);

	if (fs_handle == INVALID_HANDLE_VALUE) {
		printf("Failed to create handle for directory: %s", path_name);
		ExitProcess(GetLastError());
	}

	DWORD buff[MAX_NOTIFICATION_BUFF_SZ];

	OVERLAPPED overlapped;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ReadDirectoryChangesW(fs_handle,buff, MAX_NOTIFICATION_BUFF_SZ, TRUE,notify_filter,NULL, &overlapped, NULL );

	printf("\nWaiting for changes...\n");

	while (true) {

		DWORD result = WaitForSingleObject(overlapped.hEvent, 0);

		FILE_NOTIFY_INFORMATION *event = (FILE_NOTIFY_INFORMATION*)buff;

		switch (result) {

			case WAIT_FAILED:
				DWORD err = GetLastError();
				wprintf("An error occurred on wait event; Code: %u\n", err);
				ExitProcess(err);
				break;

			case WAIT_OBJECT_0:

				DWORD bytes_transferred;
				GetOverlappedResult(fs_handle, &overlapped, &bytes_transferred, FALSE);

				for (;;) {
					switch (event->Action) {
						case FILE_ACTION_ADDED:
							// ...
							break;

						case FILE_ACTION_REMOVED:
							// ...
							break;

						case FILE_ACTION_MODIFIED:
							// ...
							break;

						// case FILE_ACTION_RENAMED_OLD_NAME:
						// 	// ...
						// 	break;

						case FILE_ACTION_RENAMED_NEW_NAME:
							// ...
							break;

						default:
							break;
					}

					// Increment event pointer
					if (event->NextEntryOffset)
						*((uint8_t**)(&event)) += event->NextEntryOffset;
					else break;

				}
				break;
			default:
				// Unhandled Wait function
				break;

			// Queue next event
			ReadDirectoryChangesW(fs_handle,buff, FILE_NOTIFY_BUFFER_SZ, TRUE,notify_filter,NULL, &overlapped, NULL );
		}

		// ...
	}
}