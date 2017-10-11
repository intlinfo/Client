#include "stdafx.h"
#include "CrySystemVersionMatc.h"

#include <data\VersionMatch.pb.h>
#include <Poco/Process.h>

action::CrySystemVersionMatc action::CrySystemVersionMatc::Init;

void action::CrySystemVersionMatc::Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len)
{
	data::VersionMatchResponse resp;

	if (!resp.ParsePartialFromArray(data, len))
	{
		return;
	}

	if (resp.files().size() > 0)
	{
		std::vector<std::string> args;

		for (auto &url : resp.urls())
		{
			args.push_back(url);
		}

		args.push_back(":");

		for (auto &file : resp.files())
		{
			switch (file.state())
			{
			case data::ADD_FILES:
			{
				args.push_back(file.file_name());
				break;
			}
			case data::REMOVE_FILES:
			{
				break;
			}
			case data::UPDATE_FILES:
			{
				args.push_back(file.file_name());
				break;
			}
			}
		}

		std::string cmd("./Update.exe");
		Poco::ProcessHandle ph = Poco::Process::launch(cmd, args);

	}
	else
	{
#ifndef _DEBUG
		client->GetMainForm()->SetWindowText("程序无需更新...");
		Sleep(800);
		client->GetMainForm()->GetDlgItem(IDC_BUTTON_OK)->EnableWindow(TRUE);
		client->GetMainForm()->GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		client->GetMainForm()->GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
#endif
	}
}